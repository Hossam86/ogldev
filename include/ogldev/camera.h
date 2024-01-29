#pragma once
#include <ogldev/math3d.h>
#include <GLFW/glfw3.h>

namespace ogl
{
	static int MARGIN = 40;
	static float EDGE_STEP = 0.5f;
	class BasicCamera
	{
	public:
		BasicCamera() {}

		BasicCamera(int WindowWidth, int WindowHeight)
		{
			m_windowWidth = WindowWidth;
			m_windowHeight = WindowHeight;
			m_pos = Vector3f(0.0f, 0.0f, 0.0f);
			m_target = Vector3f(0.0f, 0.0f, 1.0f);
			m_up = Vector3f(0.0f, 1.0f, 0.0f);

			InitInternal();
		}

		BasicCamera(const PersProjInfo& persProjInfo, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
		{
			InitCamera(persProjInfo, Pos, Target, Up);
		}

		BasicCamera(const OrthoProjInfo& orthoProjInfo, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
		{
			InitCamera(orthoProjInfo, Pos, Target, Up);
		}

		void
		InitCamera(const PersProjInfo& persProjInfo, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
		{
			m_persProjInfo = persProjInfo;
			m_projection.InitPersProjTransform(persProjInfo);
			m_windowWidth = (int)persProjInfo.Width;
			m_windowHeight = (int)persProjInfo.Height;
			m_pos = Pos;

			m_target = Target;
			m_target.Normalize();

			m_up = Up;
			m_up.Normalize();

			InitInternal();
		}

		void
		InitCamera(const OrthoProjInfo& orthoProjInfo, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
		{
			m_projection.InitOrthoProjTransform(orthoProjInfo);
			m_windowWidth = (int)orthoProjInfo.Width;
			m_windowHeight = (int)orthoProjInfo.Height;
			m_pos = Pos;

			m_target = Target;
			m_target.Normalize();

			m_up = Up;
			m_up.Normalize();

			InitInternal();
		}

		void
		SetPosition(float x, float y, float z)
		{
			m_pos.x = x;
			m_pos.y = y;
			m_pos.z = z;
		}

		void
		SetPosition(const Vector3f& pos)
		{
			SetPosition(pos.x, pos.y, pos.z);
		}

		void
		SetTarget(float x, float y, float z)
		{
			m_target.x = x;
			m_target.y = y;
			m_target.z = z;
		}

		void
		SetTarget(const Vector3f& target)
		{
			SetTarget(target.x, target.y, target.z);
		}

		void
		SetUp(float x, float y, float z)
		{
			m_up.x = x;
			m_up.y = y;
			m_up.z = z;
		}

		bool
		OnKeyboard(int Key)
		{
			bool CameraChangedPos = false;

			switch (Key)
			{

			case GLFW_KEY_W:
				m_pos += (m_target * m_speed);
				CameraChangedPos = true;
				break;

			case GLFW_KEY_S:
				m_pos -= (m_target * m_speed);
				CameraChangedPos = true;
				break;

			case GLFW_KEY_A:
			{
				Vector3f Left = m_target.Cross(m_up);
				Left.Normalize();
				Left *= m_speed;
				m_pos += Left;
				CameraChangedPos = true;
			}
			break;

			case GLFW_KEY_D:
			{
				Vector3f Right = m_up.Cross(m_target);
				Right.Normalize();
				Right *= m_speed;
				m_pos += Right;
				CameraChangedPos = true;
			}
			break;

			case GLFW_KEY_UP:
				m_AngleV += m_speed;
				Update();
				break;

			case GLFW_KEY_DOWN:
				m_AngleV -= m_speed;
				Update();
				break;

			case GLFW_KEY_LEFT:
				m_AngleH -= m_speed;
				Update();
				break;

			case GLFW_KEY_RIGHT:
				m_AngleH += m_speed;
				Update();
				break;

			case GLFW_KEY_PAGE_UP:
				m_pos.y += m_speed;
				CameraChangedPos = true;
				break;

			case GLFW_KEY_PAGE_DOWN:
				m_pos.y -= m_speed;
				CameraChangedPos = true;
				break;

			case GLFW_KEY_KP_ADD:
				m_speed += 0.1f;
				printf("Speed changed to %f\n", m_speed);
				break;

			case GLFW_KEY_KP_SUBTRACT:
				m_speed -= 0.1f;
				if (m_speed < 0.1f)
				{
					m_speed = 0.1f;
				}
				printf("Speed changed to %f\n", m_speed);
				break;

			case GLFW_KEY_C:
				printf("Camera pos: ");
				m_pos.Print();
				printf("\n");
				printf("Camera target: ");
				m_target.Print();
				printf("\n");
				break;
			}

			if (CameraChangedPos)
			{
				//        printf("Camera pos: "); m_pos.Print(); printf("\n");
			}

			return CameraChangedPos;
		}

		void
		OnMouse(int x, int y)
		{
			int DeltaX = x - m_mousePos.x;
			int DeltaY = y - m_mousePos.y;

			m_mousePos.x = x;
			m_mousePos.y = y;

			m_AngleH += (float)DeltaX / 20.0f;
			m_AngleV += (float)DeltaY / 20.0f;

			if (x <= MARGIN)
			{
				m_OnLeftEdge = true;
				m_OnRightEdge = false;
			}
			else if (x >= (m_windowWidth - MARGIN))
			{
				m_OnRightEdge = true;
				m_OnLeftEdge = false;
			}
			else
			{
				m_OnLeftEdge = false;
				m_OnRightEdge = false;
			}

			if (y <= MARGIN)
			{
				m_OnUpperEdge = true;
				m_OnLowerEdge = false;
			}
			else if (y >= (m_windowHeight - MARGIN))
			{
				m_OnLowerEdge = true;
				m_OnUpperEdge = false;
			}
			else
			{
				m_OnUpperEdge = false;
				m_OnLowerEdge = false;
			}

			Update();
		}

		void
		OnRender()
		{
			bool ShouldUpdate = false;

			if (m_OnLeftEdge)
			{
				m_AngleH -= EDGE_STEP;
				ShouldUpdate = true;
			}
			else if (m_OnRightEdge)
			{
				m_AngleH += EDGE_STEP;
				ShouldUpdate = true;
			}

			if (m_OnUpperEdge)
			{
				if (m_AngleV > -90.0f)
				{
					m_AngleV -= EDGE_STEP;
					ShouldUpdate = true;
				}
			}
			else if (m_OnLowerEdge)
			{
				if (m_AngleV < 90.0f)
				{
					m_AngleV += EDGE_STEP;
					ShouldUpdate = true;
				}
			}

			if (ShouldUpdate)
			{
				Update();
			}
		}

		Matrix4f
		GetMatrix() const
		{
			Matrix4f CameraTransformation;
			CameraTransformation.InitCameraTransform(m_pos, m_target, m_up);

			return CameraTransformation;
		}

		const Vector3f&
		GetPos() const
		{
			return m_pos;
		}

		const Vector3f&
		GetTarget() const
		{
			return m_target;
		}

		const Vector3f&
		GetUp() const
		{
			return m_up;
		}

		const Matrix4f&
		GetProjectionMat() const
		{
			return m_projection;
		}

		const PersProjInfo&
		GetPersProjInfo() const
		{
			return m_persProjInfo;
		}

		Matrix4f
		GetViewProjMatrix() const
		{
			Matrix4f View = GetMatrix();
			Matrix4f Projection = GetProjectionMat();
			Matrix4f ViewProj = Projection * View;
			return ViewProj;
		}

		Matrix4f
		GetViewMatrix() const
		{
			return GetMatrix();
		}

		Matrix4f
		GetViewportMatrix() const
		{
			float HalfW = m_windowWidth / 2.0f;
			float HalfH = m_windowHeight / 2.0f;

			Matrix4f Viewport = Matrix4f(
				HalfW,
				0.0f,
				0.0f,
				HalfW,
				0.0f,
				HalfH,
				0.0f,
				HalfH,
				0.0f,
				0.0f,
				1.0f,
				0.0f,
				0.0f,
				0.0f,
				0.0f,
				1.0f);

			return Viewport;
		}

		void
		Print() const
		{
			printf("Pos ");
			m_pos.Print();
			printf("Target ");
			m_target.Print();
		}

		void
		SetSpeed(float Speed)
		{
			if (Speed <= 0.0f)
			{
				printf("Invalid camera speed %f\n", Speed);
				exit(0);
			}

			m_speed = Speed;
		}

		void
		SetName(const std::string& Name)
		{
			m_name = Name;
		}

		const std::string&
		GetName() const
		{
			return m_name;
		}

	private:
		void
		InitInternal()
		{
			Vector3f HTarget(m_target.x, 0.0, m_target.z);
			HTarget.Normalize();

			float Angle = ToDegree(asin(abs(HTarget.z)));

			if (HTarget.z >= 0.0f)
			{
				if (HTarget.x >= 0.0f)
				{
					m_AngleH = 360.0f - Angle;
				}
				else
				{
					m_AngleH = 180.0f + Angle;
				}
			}
			else
			{
				if (HTarget.x >= 0.0f)
				{
					m_AngleH = Angle;
				}
				else
				{
					m_AngleH = 180.0f - Angle;
				}
			}

			m_AngleV = -ToDegree(asin(m_target.y));

			m_OnUpperEdge = false;
			m_OnLowerEdge = false;
			m_OnLeftEdge = false;
			m_OnRightEdge = false;
			m_mousePos.x = m_windowWidth / 2;
			m_mousePos.y = m_windowHeight / 2;
		}
		void
		Update()
		{
			Vector3f Yaxis(0.0f, 1.0f, 0.0f);

			// Rotate the view vector by the horizontal angle around the vertical axis
			Vector3f View(1.0f, 0.0f, 0.0f);
			View.Rotate(m_AngleH, Yaxis);
			View.Normalize();

			// Rotate the view vector by the vertical angle around the horizontal axis
			Vector3f U = Yaxis.Cross(View);
			U.Normalize();
			View.Rotate(m_AngleV, U);

			m_target = View;
			m_target.Normalize();

			m_up = m_target.Cross(U);
			m_up.Normalize();
		}

		std::string m_name;

		Vector3f m_pos;
		Vector3f m_target;
		Vector3f m_up;

		float m_speed = 0.1f;
		int m_windowWidth;
		int m_windowHeight;

		float m_AngleH;
		float m_AngleV;

		bool m_OnUpperEdge;
		bool m_OnLowerEdge;
		bool m_OnLeftEdge;
		bool m_OnRightEdge;

		Vector2i m_mousePos;

		PersProjInfo m_persProjInfo;
		Matrix4f m_projection;
	};
} // namespace ogl