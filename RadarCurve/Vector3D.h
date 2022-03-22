#pragma once

static double s_Radius = 6378137;
static double s_PI = 3.1415926;
static double s_HUs_DU = s_PI /180.0;
static double s_DU = 180.0 /s_PI;

class CVector3D
{
public:
	CVector3D() {}

	double X;
	double Y;
	double Z;

	/// 向量长度
	double Length()const
	{
		return sqrt(X*X + Y*Y + Z*Z);
	}

	/// 转变为单位向量
	void Normalize()
	{
		double dLength = Length();
		if (dLength >= 0)
		{
			X /= dLength;
			Y /= dLength;
			Z /= dLength;
		}
	}

	CVector3D operator +(CVector3D vRight)const
	{
		CVector3D cVector3D;
		cVector3D.X = X + vRight.X;
		cVector3D.Y = Y + vRight.Y;
		cVector3D.Z = Z + vRight.Z;

		return cVector3D;
	}

	/// 向量减法
	CVector3D operator -(CVector3D vRight)const
	{
		CVector3D cVector3D;
		cVector3D.X = X - vRight.X;
		cVector3D.Y = Y - vRight.Y;
		cVector3D.Z = Z - vRight.Z;

		return cVector3D;
	}

	/// 向量乘法
	CVector3D operator *(double dScale)const
	{
		CVector3D cVector3D;
		cVector3D.X = X *dScale;
		cVector3D.Y = Y *dScale;
		cVector3D.Z = Z *dScale;

		return cVector3D;
	}

	/// 向量除法
	CVector3D operator /(double dScale)const
	{
		CVector3D cVector3D;
		cVector3D.X = X / dScale;
		cVector3D.Y = Y / dScale;
		cVector3D.Z = Z / dScale;

		return cVector3D;
	}

	void Angle2Vector3D()
	{
		CVector3D cVec3D;

		double dZinXY = (Z + s_Radius) * cos(Y *s_HUs_DU);
		cVec3D.Z = (Z + s_Radius) * sin(Y *s_HUs_DU);
		cVec3D.X = dZinXY * cos(X *s_HUs_DU);
		cVec3D.Y = dZinXY * sin(X *s_HUs_DU);

		X = cVec3D.X;
		Y = cVec3D.Y;
		Z = cVec3D.Z;
	}

	CVector3D Angle2Vector3D(CVector3D sAngle)
	{
		CVector3D cVec3D;
		double dZinXY = (sAngle.Z + s_Radius) * cos(sAngle.Y *s_HUs_DU);
		cVec3D.Z = (sAngle.Z + s_Radius) * sin(sAngle.Y *s_HUs_DU);
		cVec3D.X = dZinXY * cos(sAngle.X *s_HUs_DU);
		cVec3D.Y = dZinXY * sin(sAngle.X *s_HUs_DU);

		return cVec3D;
	}

	CVector3D Vector3D2Angle(CVector3D cVector3D)
	{
		CVector3D sAngle;

		// 经度（分象限计算）
		if (cVector3D.X >= 0 && cVector3D.Y >= 0)
		{
			sAngle.X = atan(cVector3D.Y / cVector3D.X);
		}
		else if (cVector3D.X < 0 && cVector3D.Y > 0)
		{
			sAngle.X = atan(cVector3D.Y / cVector3D.X);
			sAngle.X += s_PI;
		}
		else if (cVector3D.X < 0 && cVector3D.Y < 0)
		{
			sAngle.X = atan(cVector3D.Y / cVector3D.X);
			sAngle.X = -sAngle.X;
		}
		else if (cVector3D.X > 0 && cVector3D.Y < 0)
		{
			sAngle.X = atan(cVector3D.Y / cVector3D.X);
		}

		// 纬度（分象限计算）
		sAngle.Y = atan(cVector3D.Z * cos(sAngle.X) / cVector3D.X);
		if (cVector3D.Z > 0)
		{
			sAngle.Y = fabs(atan(cVector3D.Z * cos(sAngle.X) / cVector3D.X));
		}
		else if (cVector3D.Z < 0)
		{
			sAngle.Y = -fabs(atan(cVector3D.Z * cos(sAngle.X) / cVector3D.X));
		}

		// 高度
		sAngle.Z = cVector3D.Length() - s_Radius;

		// 转换成度
		sAngle.X *= s_DU;
		sAngle.Y *= s_DU;

		return sAngle;
	}

	void Compute(CVector3D* pAngleBuf, const CVector3D& sAngleStart, const CVector3D& sAngleEnd, const int& iInsertNum)
	{
		if (iInsertNum == 0)
		{
			return;
		}
		if (iInsertNum <= 1)
		{
			pAngleBuf[0] = sAngleStart;
			return;
		}

		int iAngleBufIdx = 0;
		while (iAngleBufIdx < iInsertNum)
		{
			pAngleBuf[iAngleBufIdx] = sAngleStart + (sAngleEnd-sAngleStart) *(double(iAngleBufIdx) /(iInsertNum-1));
			iAngleBufIdx++;
		}

		pAngleBuf[iInsertNum-1] = sAngleEnd;
	}

	/*void Compute(CVector3D* pAngleBuf, const CVector3D& sAngleStart, const CVector3D& sAngleEnd, const int& iInsertNum)
	{
		if (iInsertNum <= 1)
		{
			return;
		}

		CVector3D cVecStart = Angle2Vector3D(sAngleStart);
		CVector3D cVecEnd = Angle2Vector3D(sAngleEnd);
		CVector3D cVecNormalize = cVecEnd - cVecStart;

		double dLength = cVecNormalize.Length();
		cVecNormalize.Normalize();
		double dInterval = dLength / (iInsertNum-1);

		pAngleBuf[0] = sAngleStart;
		double dCurrentLength = dInterval;
		int iAngleBufIdx = 1;

		while (iAngleBufIdx < iInsertNum)
		{
			CVector3D cTempVector3D = cVecStart + cVecNormalize * dCurrentLength;
			pAngleBuf[iAngleBufIdx] = Vector3D2Angle(cTempVector3D);

			dCurrentLength += dInterval;
			iAngleBufIdx++;
		}

		pAngleBuf[iInsertNum-1] = sAngleEnd;
	}*/
};