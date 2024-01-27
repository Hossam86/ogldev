#pragma once
#include <ogldev/math3d.h>

namespace ogl
{

	class WorldTrans
	{
	public:
		WorldTrans() {}

		void
		SetScale(float scale);
		void
		SetRotation(float x, float y, float z);
		void
		SetRotation(const Vector3f&);
		void
		SetPosition(float x, float y, float z);
		void
		SetPosition(const Vector3f& WorldPos);

		void
		Rotate(float x, float y, float z);

		Matrix4f
		GetMatrix() const;

		Vector3f
		WorldPosToLocalPos(const Vector3f& WorldPos) const;
		Vector3f
		WorldDirToLocalDir(const Vector3f& WorldDir) const;

		Matrix4f
		GetReversedTranslationMatrix() const;
		Matrix4f
		GetReversedRotationMatrix() const;

		float
		GetScale() const
		{
			return m_scale;
		}
		Vector3f
		GetPos() const
		{
			return m_pos;
		}
		Vector3f
		GetRotation() const
		{
			return m_rotation;
		}

	private:
		float m_scale = 1.0f;
		Vector3f m_rotation = Vector3f(0.0f, 0.0f, 0.0f);
		Vector3f m_pos = Vector3f(0.0f, 0.0f, 0.0f);
	};

	// IMP
	void
	WorldTrans::SetScale(float scale)
	{
		m_scale = scale;
	}

	void
	WorldTrans::SetRotation(float x, float y, float z)
	{
		m_rotation.x = x;
		m_rotation.y = y;
		m_rotation.z = z;
	}

	void
	WorldTrans::SetPosition(float x, float y, float z)
	{
		m_pos.x = x;
		m_pos.y = y;
		m_pos.z = z;
	}

	void
	WorldTrans::SetPosition(const Vector3f& WorldPos)
	{
		m_pos = WorldPos;
	}

	void
	WorldTrans::Rotate(float x, float y, float z)
	{
		m_rotation.x += x;
		m_rotation.y += y;
		m_rotation.z += z;
	}

	Matrix4f
	WorldTrans::GetMatrix() const
	{
		Matrix4f Scale;
		Scale.InitScaleTransform(m_scale, m_scale, m_scale);

		Matrix4f Rotation;
		Rotation.InitRotateTransform(m_rotation.x, m_rotation.y, m_rotation.z);

		Matrix4f Translation;
		Translation.InitTranslationTransform(m_pos.x, m_pos.y, m_pos.z);

		Matrix4f WorldTransformation = Translation * Rotation * Scale;

		return WorldTransformation;
	}

	Matrix4f
	WorldTrans::GetReversedTranslationMatrix() const
	{
		Matrix4f ReversedTranslation;
		ReversedTranslation.InitTranslationTransform(m_pos.Negate());
		return ReversedTranslation;
	}

	Matrix4f
	WorldTrans::GetReversedRotationMatrix() const
	{
		Matrix4f ReversedRotation;
		ReversedRotation.InitRotateTransformZYX(-m_rotation.x, -m_rotation.y, -m_rotation.z);
		return ReversedRotation;
	}

	Vector3f
	WorldTrans::WorldPosToLocalPos(const Vector3f& WorldPos) const
	{
		Matrix4f WorldToLocalTranslation = GetReversedTranslationMatrix();
		Matrix4f WorldToLocalRotation = GetReversedRotationMatrix();
		Matrix4f WorldToLocalTransformation = WorldToLocalRotation * WorldToLocalTranslation;
		Vector4f WorldPos4f = Vector4f(WorldPos, 1.0f);
		Vector4f LocalPos4f = WorldToLocalTransformation * WorldPos4f;
		Vector3f LocalPos3f(LocalPos4f);
		return LocalPos3f;
	}

	Vector3f
	WorldTrans::WorldDirToLocalDir(const Vector3f& WorldDirection) const
	{
		Matrix3f World3f(GetMatrix()); // Initialize using the top left corner

		// Inverse local-to-world transformation using transpose
		// (assuming uniform scaling)
		Matrix3f WorldToLocal = World3f.Transpose();

		Vector3f LocalDirection = WorldToLocal * WorldDirection;

		LocalDirection = LocalDirection.Normalize();

		return LocalDirection;
	}
}
