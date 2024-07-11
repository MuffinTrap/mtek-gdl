#include <GL/gl.h>
#include <GL/glu.h>
#include <gccore.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <gctypes.h>



void gluPerspective(GLdouble fovy,GLdouble aspect, GLdouble zNear, GLdouble zFar) {
	GLfloat m[4][4];
	GLfloat sine, cotangent, deltaZ;
	GLfloat radians = fovy * (float)(1.0f / 2.0f * M_PI / 180.0f);

	deltaZ = zFar - zNear;
	sine = sinf(radians);
	if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) return;

	cotangent = cosf(radians) / sine;

	m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
	m[1][0] = 0; m[1][2] = 0; m[1][3] = 0;
	m[2][0] = 0; m[2][1] = 0;
	m[3][0] = 0; m[3][1] = 0;
	m[0][0] = cotangent / aspect;
	m[1][1] = cotangent;
	m[2][2] = -(0 + zNear) / deltaZ;
	m[2][3] = -1;
	m[3][2] = - zNear * zFar / deltaZ;
	m[3][3] = 0;

	glMultMatrixf((float*)m);
}
void _normalize(GLfloat v[3]) {
    GLfloat r;
    r=(GLfloat)sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if (r==0.0f) return;

    v[0]/=r; v[1]/=r; v[2]/=r;
}
void _cross(GLfloat v1[3], GLfloat v2[3], GLfloat result[3]) {
    result[0] = v1[1]*v2[2] - v1[2]*v2[1];
    result[1] = v1[2]*v2[0] - v1[0]*v2[2];
    result[2] = v1[0]*v2[1] - v1[1]*v2[0];
}
void gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,
          GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy, GLdouble upz) {

	GLfloat forward[3], side[3], up[3];
	GLfloat m[4][4];

	forward[0] = centerx - eyex;
	forward[1] = centery - eyey;
	forward[2] = centerz - eyez;

	up[0] = upx; up[1] = upy; up[2] = upz;

	_normalize(forward);
	_cross(forward, up, side);
	_normalize(side);
	_cross(side, forward, up);

	m[0][3] = 0; m[1][3] = 0; m[2][3] = 0;
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
	m[0][0] = side[0];     m[1][0] = side[1];     m[2][0] = side[2];
	m[0][1] = up[0];       m[1][1] = up[1];       m[2][1] = up[2];
	m[0][2] = -forward[0]; m[1][2] = -forward[1]; m[2][2] = -forward[2];

	glMultMatrixf(&m[0][0]);
	glTranslatef(-eyex, -eyey, -eyez);
}

// Always returns error!
GLint gluBuild2DMipmaps (GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *data) {
	int level = 0;
	int bpp = 4;
	if (format == GL_RGB || format == GL_BGR) {
			bpp = 3;
	}
	unsigned char * buf = malloc(width * height * bpp);
	int w = width, h = height;

	while (w > 1 && h > 1) {
		gluScaleImage(format,width, height, type, data, w, h, type, buf);
		glTexImage2D(target, level, internalFormat, w, h, 0, format, type, buf);

		if (w > 1) w /= 2;
		if (h > 1) h /= 2;
		level++;
	}
	free(buf);
	return 0;
}

//// Image scaling for arbitrary size taken from Mesa 3D and adapted by davidgf ////

void scale_internal(int components, int widthin, int heightin,const unsigned char *datain,
			   int widthout, int heightout,unsigned char *dataout) {
    float x, lowx, highx, convx, halfconvx;
    float y, lowy, highy, convy, halfconvy;
    float xpercent,ypercent;
    float percent;
    /* Max components in a format is 4, so... */
    float totals[4];
    float area;
    int i,j,k,yint,xint,xindex,yindex;
    int temp;

    convy = (float) heightin/heightout;
    convx = (float) widthin/widthout;
    halfconvx = convx/2;
    halfconvy = convy/2;
    for (i = 0; i < heightout; i++) {
		y = convy * (i+0.5);
		if (heightin > heightout) {
			highy = y + halfconvy;
			lowy = y - halfconvy;
		} else {
			highy = y + 0.5;
			lowy = y - 0.5;
		}
		for (j = 0; j < widthout; j++) {
			x = convx * (j+0.5);
			if (widthin > widthout) {
				highx = x + halfconvx;
				lowx = x - halfconvx;
			} else {
				highx = x + 0.5;
				lowx = x - 0.5;
			}

			/*
			** Ok, now apply box filter to box that goes from (lowx, lowy)
			** to (highx, highy) on input data into this pixel on output
			** data.
			*/
			totals[0] = totals[1] = totals[2] = totals[3] = 0.0;
			area = 0.0;

			y = lowy;
			yint = floor(y);
			while (y < highy) {
				yindex = (yint + heightin) % heightin;
				if (highy < yint+1) {
					ypercent = highy - y;
				} else {
					ypercent = yint+1 - y;
				}

				x = lowx;
				xint = floor(x);

				while (x < highx) {
					xindex = (xint + widthin) % widthin;
					if (highx < xint+1) {
						xpercent = highx - x;
					} else {
						xpercent = xint+1 - x;
					}

					percent = xpercent * ypercent;
					area += percent;
					temp = (xindex + (yindex * widthin)) * components;
					for (k = 0; k < components; k++) {
						totals[k] += datain[temp + k] * percent;
					}

					xint++;
					x = xint;
				}
				yint++;
				y = yint;
		    }

			temp = (j + (i * widthout)) * components;
			for (k = 0; k < components; k++) {
				/* totals[] should be rounded in the case of enlarging an RGB
				 * ramp when the type is 332 or 4444
				 */
				dataout[temp + k] = (totals[k]+0.5)/area;
			}
		}
    }
}
// is not a full implementation and thus has useless parameters, be warned!!
GLint gluScaleImage (GLenum format, GLsizei wIn, GLsizei hIn, __attribute__((unused)) GLenum typeIn, const void *dataIn, GLsizei wOut, GLsizei hOut, __attribute__((unused)) GLenum typeOut, GLvoid* dataOut) {
	switch (format) {
	case GL_RGB:
	case GL_BGR:
		scale_internal(3,wIn,hIn,dataIn,wOut,hOut,dataOut);
		return 0;
	case GL_RGBA:
	case GL_BGRA:
		scale_internal(4,wIn,hIn,dataIn,wOut,hOut,dataOut);
		return 0;
	};
	return -1;
}