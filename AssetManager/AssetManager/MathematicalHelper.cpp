#include "pch.h"
#include "MathematicalHelper.h"

using namespace DirectX;

XMFLOAT3 Normalize(const XMFLOAT3& Vector)
{
	float Length = sqrt(Vector.x * Vector.x + Vector.y * Vector.y + Vector.z * Vector.z);
	return XMFLOAT3{ Vector.x / Length, Vector.y / Length, Vector.z / Length };
}

float MathematicalHelper::InnerProduct(const XMFLOAT3& Vector1, const XMFLOAT3& Vector2)
{
	return Vector1.x * Vector2.x + Vector1.y * Vector2.y + Vector1.z * Vector2.z;
}

XMFLOAT3 MathematicalHelper::Projection(const XMFLOAT3& ToVector, const XMFLOAT3& FromVector)
{
	float ProjLength = (InnerProduct(ToVector, FromVector) / InnerProduct(ToVector, ToVector));
	return XMFLOAT3{ ToVector.x * ProjLength, ToVector.y * ProjLength, ToVector.z * ProjLength };
}

void MathematicalHelper::GramShmidt(
	const XMFLOAT3& Vector1In,
	const XMFLOAT3& Vector2In,
	const XMFLOAT3& Vector3In,
	XMFLOAT3& Vector2Out,
	XMFLOAT3& Vector3Out
)
{
	const XMFLOAT3 Proj12 = Projection(Vector1In, Vector2In);
	Vector2Out = XMFLOAT3{
		Vector2In.x - Proj12.x,
		Vector2In.y - Proj12.y,
		Vector2In.z - Proj12.z
	};
	Vector2Out = Normalize(Vector2Out);

	const XMFLOAT3 Proj13 = Projection(Vector1In, Vector3In);
	const XMFLOAT3 Proj2out3 = Projection(Vector2Out, Vector3In);
	Vector3Out = XMFLOAT3{
		Vector3In.x - Proj13.x - Proj2out3.x,
		Vector3In.y - Proj13.y - Proj2out3.y,
		Vector3In.z - Proj13.z - Proj2out3.z
	};
	Vector3Out = Normalize(Vector3Out);
}

void MathematicalHelper::GetTangentBitangent(
	const XMFLOAT3& Pos0In,
	const XMFLOAT3& Pos1In,
	const XMFLOAT3& Pos2In,
	const XMFLOAT2& TexCoord0In,
	const XMFLOAT2& TexCoord1In,
	const XMFLOAT2& TexCoord2In,
	const XMFLOAT3& Normal0In,
	const XMFLOAT3& Normal1In,
	const XMFLOAT3& Normal2In,
	XMFLOAT3& Tangent0Out,
	XMFLOAT3& Tangent1Out,
	XMFLOAT3& Tangent2Out,
	XMFLOAT3& Bitangent0Out,
	XMFLOAT3& Bitangent1Out,
	XMFLOAT3& Bitangent2Out
)
{
	XMFLOAT3 DeltaPos1 = XMFLOAT3{ Pos1In.x - Pos0In.x, Pos1In.y - Pos0In.y, Pos1In.z - Pos0In.z };
	XMFLOAT3 DeltaPos2 = XMFLOAT3{ Pos2In.x - Pos0In.x, Pos2In.y - Pos0In.y, Pos2In.z - Pos0In.z };

	XMFLOAT2 DeltaUV1 = XMFLOAT2{ TexCoord1In.x - TexCoord0In.x, TexCoord1In.y - TexCoord0In.y };
	XMFLOAT2 DeltaUV2 = XMFLOAT2{ TexCoord2In.x - TexCoord0In.x, TexCoord2In.y - TexCoord0In.y };

	float Determinant = 1.0f / (DeltaUV1.x * DeltaUV2.y - DeltaUV1.y * DeltaUV2.x);

	XMVECTOR vDeltaPos1 = XMLoadFloat3(&DeltaPos1);
	XMVECTOR vDeltaPos2 = XMLoadFloat3(&DeltaPos2);

	XMVECTOR vTangent = (vDeltaPos1 * DeltaUV2.y - vDeltaPos2 * DeltaUV1.y) * Determinant;
	XMVECTOR vBitangent = (vDeltaPos2 * DeltaUV1.x - vDeltaPos1 * DeltaUV2.x) * Determinant;

	vTangent = XMVector3Normalize(vTangent);
	vBitangent = XMVector3Normalize(vBitangent);

	XMFLOAT3 Tangent;
	XMFLOAT3 Bitangent;

	XMStoreFloat3(&Tangent, vTangent);
	XMStoreFloat3(&Bitangent, vBitangent);

	GramShmidt(
		Normal0In,
		Tangent,
		Bitangent,
		Tangent0Out,
		Bitangent0Out
	);
	GramShmidt(
		Normal1In,
		Tangent,
		Bitangent,
		Tangent1Out,
		Bitangent1Out
	);
	GramShmidt(
		Normal2In,
		Tangent,
		Bitangent,
		Tangent2Out,
		Bitangent2Out
	);
}

