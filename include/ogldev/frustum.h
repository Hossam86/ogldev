#pragma once

#include "ogldev/vec3f.h"
#include <ogldev/AABB.h>
#include <ogldev/mat4f.h>

class Frustum
{
public:
	Vector4f NearTopLeft;
	Vector4f NearBottomLeft;
	Vector4f NearTopRight;
	Vector4f NearBottomRight;

	Vector4f FarTopLeft;
	Vector4f FarBottomLeft;
	Vector4f FarTopRight;
	Vector4f FarBottomRight;

	Frustum() {}

	void
	CalcCorners(const PersProjInfo& persProjInfo)
	{
		float AR = persProjInfo.Height / persProjInfo.Width;

		float tanHalfFOV = tanf(ToRadian(persProjInfo.FOV / 2.0f));

		float NearZ = persProjInfo.zNear;
		float NearX = NearZ * tanHalfFOV;
		float NearY = NearZ * tanHalfFOV * AR;

		NearTopLeft = Vector4f(-NearX, NearY, NearZ, 1.0f);
		NearBottomLeft = Vector4f(-NearX, -NearY, NearZ, 1.0f);
		NearTopRight = Vector4f(NearX, NearY, NearZ, 1.0f);
		NearBottomRight = Vector4f(NearX, -NearY, NearZ, 1.0f);

		float FarZ = persProjInfo.zFar;
		float FarX = FarZ * tanHalfFOV;
		float FarY = FarZ * tanHalfFOV * AR;

		FarTopLeft = Vector4f(-FarX, FarY, FarZ, 1.0f);
		FarBottomLeft = Vector4f(-FarX, -FarY, FarZ, 1.0f);
		FarTopRight = Vector4f(FarX, FarY, FarZ, 1.0f);
		FarBottomRight = Vector4f(FarX, -FarY, FarZ, 1.0f);
	}

	void
	Transform(const Matrix4f& m)
	{
		NearTopLeft = m * NearTopLeft;
		NearBottomLeft = m * NearBottomLeft;
		NearTopRight = m * NearTopRight;
		NearBottomRight = m * NearBottomRight;

		FarTopLeft = m * FarTopLeft;
		FarBottomLeft = m * FarBottomLeft;
		FarTopRight = m * FarTopRight;
		FarBottomRight = m * FarBottomRight;
	}

	void
	CalcAABB(AABB& aabb)
	{
		aabb.Add(NearTopLeft);
		aabb.Add(NearBottomLeft);
		aabb.Add(NearTopRight);
		aabb.Add(NearBottomRight);

		aabb.Add(FarTopLeft);
		aabb.Add(FarBottomLeft);
		aabb.Add(FarTopRight);
		aabb.Add(FarBottomRight);
	}

	void
	Print()
	{
		printf("NearTopLeft ");
		NearTopLeft.Print();
		printf("NearBottomLeft ");
		NearBottomLeft.Print();
		printf("NearTopRight ");
		NearTopRight.Print();
		printf("NearBottomLeft ");
		NearBottomRight.Print();

		printf("FarTopLeft ");
		FarTopLeft.Print();
		printf("FarBottomLeft ");
		FarBottomLeft.Print();
		printf("FarTopRight ");
		FarTopRight.Print();
		printf("FarBottomLeft ");
		FarBottomRight.Print();
	}
};

class FrustumCulling
{
public:
	FrustumCulling(const Matrix4f& ViewProj) { Update(ViewProj); }

	void
	Update(const Matrix4f& ViewProj)
	{
		ViewProj.CalcClipPlanes(
			m_leftClipPlane,
			m_rightClipPlane,
			m_bottomClipPlane,
			m_topClipPlane,
			m_nearClipPlane,
			m_farClipPlane);
	}

	bool
	IsPointInsideViewFrustum(const Vector3f& p) const
	{
		Vector4f p4D(p, 1.0f);

		bool Inside = (m_leftClipPlane.Dot(p4D) >= 0) && (m_rightClipPlane.Dot(p4D) <= 0) &&
					  //   (m_topClipPlane.Dot(p4D)    <= 0) &&
					  //   (m_bottomClipPlane.Dot(p4D) >= 0) &&
					  (m_nearClipPlane.Dot(p4D) >= 0) && (m_farClipPlane.Dot(p4D) <= 0);

		return Inside;
	}

private:
	Vector4f m_leftClipPlane;
	Vector4f m_rightClipPlane;
	Vector4f m_bottomClipPlane;
	Vector4f m_topClipPlane;
	Vector4f m_nearClipPlane;
	Vector4f m_farClipPlane;
};

void
CalcTightLightProjection(
	const Matrix4f& CameraView,		  // in
	const Vector3f& LightDir,		  // in
	const PersProjInfo& persProjInfo, // in
	Vector3f& LightPosWorld,		  // out
	OrthoProjInfo& orthoProjInfo)	  // out
{
	//
	// Step #1: Calculate the view frustum corners in view space
	//
	Frustum frustum;
	frustum.CalcCorners(persProjInfo);

	//
	// Step #2: transform the view frustum to world space
	//
	Matrix4f InverseCameraView = CameraView.Inverse();
	frustum.Transform(InverseCameraView);

	Frustum view_frustum_in_world_space = frustum; // backup for later

	//
	// Step #3: Transform the view frustum to light space (1st time)
	//
	Matrix4f LightView;
	Vector3f Origin(0.0f, 0.0f, 0.0f);
	Vector3f Up(0.0f, 1.0f, 0.0f);
	LightView.InitCameraTransform(Origin, LightDir, Up);
	frustum.Transform(LightView);

	//
	// Step #4: Calculate an AABB for the view frustum in light space
	//
	AABB aabb;
	frustum.CalcAABB(aabb);

	//
	// Step #5: Calculate the position of the light
	//
	Vector3f BottomLeft(aabb.MinX, aabb.MinY, aabb.MinZ);
	Vector3f TopRight(aabb.MaxX, aabb.MaxY, aabb.MinZ);
	Vector4f LightPosWorld4d = Vector4f((BottomLeft + TopRight) / 2.0f, 1.0f);

	//
	// Step #6: transform the position of the light back to world space
	//
	Matrix4f LightViewInv = LightView.Inverse();
	LightPosWorld4d = LightViewInv * LightPosWorld4d;
	LightPosWorld = Vector3f(LightPosWorld4d.x, LightPosWorld4d.y, LightPosWorld4d.z);

	//
	// Step #7: transform the view frustum to light space (2nd time)
	//
	LightView.InitCameraTransform(LightPosWorld, LightDir, Up);
	view_frustum_in_world_space.Transform(LightView);

	//
	// Step #8: with the light in its final position recalculate the aabb
	//
	AABB final_aabb;
	view_frustum_in_world_space.CalcAABB(final_aabb);
	final_aabb.UpdateOrthoInfo(orthoProjInfo);
}

bool
IsPointInsideViewFrustum(const Vector3f& p, const Matrix4f& VP)
{
	Vector4f p4D(p, 1.0f);

	Vector4f ClipSpaceP = VP * p4D;

	bool InsideViewFrustum =
		((ClipSpaceP.x <= ClipSpaceP.w) && (ClipSpaceP.x >= -ClipSpaceP.w) && (ClipSpaceP.y <= ClipSpaceP.w) &&
		 (ClipSpaceP.y >= -ClipSpaceP.w) && (ClipSpaceP.z <= ClipSpaceP.w) && (ClipSpaceP.z >= -ClipSpaceP.w));

	return InsideViewFrustum;
}