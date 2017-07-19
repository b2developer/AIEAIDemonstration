#include "Transform.h"

Matrix3 Transform::getMatrix()
{
	//create a transformation matrix for the box min/max coordinates
	Matrix3 transMat = Matrix3();
	Matrix3 rotMat = Matrix3();
	Matrix3 scaleMat = Matrix3();

	//the scale, rotation and translation matrices
	scaleMat.setScale(scale);
	rotMat.setRotateZ(rotation);
	transMat.setTranslate(translation);

	//combine the matrices into a final transformation
	Matrix3 combinedMat = transMat * rotMat * scaleMat;

	return combinedMat;
}
