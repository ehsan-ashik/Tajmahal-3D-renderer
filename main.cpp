#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<windows.h>
#include<GL/glut.h>
#include<string>

#define BLACK 0, 0, 0

//global variables -- for tracking the anglular position of camera
double cameraAngle;	
double cameraAngleRotate;	//in radian
double cameraAngleDelta;

double cameraHeight;	
double cameraRadius;

double rectAngle;	//in degree

//camera vars
double fdVector[3] = {0,-1,0};
double upVector[3] = {0,0,1};
double alongVector[3] = {1,0,0};
double position[3] = {0, -100, 10};
double lookat[3] = {0,0,10};
double up[3] = {0,0,1};
double angle = 1;
double offset = 10;
//camera vars ends

GLUquadric* IDquadric=gluNewQuadric() ;
GLuint pillarWhiteBig, frontDoor, wallTex, wallTexRound, gombujChoto , domeBase1, domeBase2, domeBase3, domeBase4, gombujBoro,
		chotoPillar, front1, front2, front3, frontTop, frontSide, frontTop1, frontSide1 , redBrick, base;
int num_texture = -1;

GLfloat global_ambient_day[] = { .8, .8, .862745, 1.0 };
GLfloat global_ambient_day1[] = { 1, 1, .862745, 1.0 };//white light
			GLfloat global_ambient_night[] = { .2, .2, .2, 1.0 }; //white light

			//defuse
	GLfloat diffuseDirClr[] = {.5, .5, .5, 1.0}; //Color (0.5, 0.5, 0.5)
	 GLfloat lightDir[] = {-1.0, 0.5, 10, 0.0};
	 //defusepoint light
	 GLfloat diffusePoint_white[] = {0.5, 0.5, 0.5, 1.0}; //Color (0.5, 0.5, 0.5)
	 GLfloat diffusePoint_Blue[] = {0, 0, 1, 1.0};
	 GLfloat position_light[] = {-100.0, -100.0, 50.0, 1.0};
	 GLfloat position_light_fixed[] = {-100.0, -100.0, 40.0, 1.0};
	 //spotlight
	 GLfloat light_spot[] = { 1.0, 1.0, 1.0, 1.0 };
	 GLfloat lightDir_spot[] = {15.0, 0, 10, 1.0};
	 GLfloat lightDir_spot1[] = {0, 0, 0, 1.0};


bool canDrawGrid;

int LoadBitmap(std::string filename)
{
	// convert string to char*
	const char *file = filename.c_str();

    int i, j=0;
    FILE *l_file;
    unsigned char *l_texture;

    BITMAPFILEHEADER fileheader;
    BITMAPINFOHEADER infoheader;
    RGBTRIPLE rgb;

    num_texture++;

    if( (l_file = fopen(file, "rb"))==NULL) return (-1);

    fread(&fileheader, sizeof(fileheader), 1, l_file);

    fseek(l_file, sizeof(fileheader), SEEK_SET);
    fread(&infoheader, sizeof(infoheader), 1, l_file);

    l_texture = (byte *) malloc(infoheader.biWidth * infoheader.biHeight * 4);
    memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);

	for (i=0; i < infoheader.biWidth*infoheader.biHeight; i++)
	{
		fread(&rgb, sizeof(rgb), 1, l_file);

		l_texture[j+0] = rgb.rgbtRed;
		l_texture[j+1] = rgb.rgbtGreen;
		l_texture[j+2] = rgb.rgbtBlue;
		l_texture[j+3] = 255;
		j += 4;
	}	
    fclose(l_file);

    glBindTexture(GL_TEXTURE_2D, num_texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	// glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, infoheader.biWidth, infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, infoheader.biWidth, infoheader.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

    free(l_texture);

    return (num_texture);

}

void loadAssets(void)
{

	pillarWhiteBig =LoadBitmap("brick.bmp");   /*here bkg1.bmp is the bitmap image to be used as texture, texid is global varible declared to uniquely  identify this particular image*/
    frontDoor =LoadBitmap("assets/front.jpg"); 
	wallTex=LoadBitmap("assets/niche_1.bmp");
	wallTexRound = LoadBitmap("assets/niche_3.bmp");
	gombujChoto = LoadBitmap("assets/Ny-kupolmapping.bmp");
	domeBase1 = LoadBitmap("assets/TM-domebasedec1.bmp");
	domeBase2 = LoadBitmap("assets/TM-domebase.bmp");
	domeBase3 = LoadBitmap("assets/TM-domebase1.bmp");
	domeBase4 = LoadBitmap("assets/TM-domebase2.bmp");
	gombujBoro = LoadBitmap("assets/boroGombuj.bmp");
	chotoPillar = LoadBitmap("assets/pillar.bmp");
	front1 = LoadBitmap("assets/front_1.bmp");
	front2 = LoadBitmap("assets/front_2.bmp");
	front3 = LoadBitmap("assets/front_3.bmp");
	frontTop = LoadBitmap("assets/front_top.bmp");
	frontSide = LoadBitmap("assets/front_side.bmp");
	frontTop1 = LoadBitmap("assets/front_top1.bmp");
	frontSide1 = LoadBitmap("assets/front_side1.bmp");
	redBrick = LoadBitmap("assets/Brick_Pavers_Modular.bmp");
	base = LoadBitmap("assets/base.bmp");
}

void pillar(){
	int i;
			
	glPushMatrix(); {
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, wallTexRound); // here texid corresponds a bitmap image.
			gluQuadricNormals(IDquadric,GLU_SMOOTH);
			gluQuadricTexture(IDquadric, GLU_TRUE);
			gluCylinder(IDquadric,4 ,4 ,11,8,8);
		glDisable(GL_TEXTURE_2D);
	}glPopMatrix();

	glPushMatrix(); {
		glTranslatef(0,0,11);
		glutSolidCone(4,.001, 8, 8);
	}glPopMatrix();
	
	//pillar
	glPushMatrix(); {
		glTranslatef(0,0,10);
		//glColor3f(.6,.6,.6);
		glPushMatrix(); {
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, pillarWhiteBig); // here texid corresponds a bitmap image.
			gluQuadricNormals(IDquadric,GLU_SMOOTH);
			gluQuadricTexture(IDquadric, GLU_TRUE);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			gluCylinder(IDquadric,2.5, 1.5, 40, 50, 50);
			glDisable(GL_TEXTURE_2D);
		}glPopMatrix();
		
		//curve 1
		glPushMatrix(); {
			glTranslatef(0,0,13);
			glColor3f(.9,.9,.9);
			gluCylinder(IDquadric, 2.9, 2.9, .5, 20, 20);
			glutSolidCone(2.9,.001, 20, 20);
			
			glPushMatrix(); {
				glTranslatef(0,0,.5);
				glColor3f(.8,.8,.8);
				GLUquadricObj *quadratic;
				quadratic = gluNewQuadric();
				gluCylinder(quadratic, 2.9, 2.9, 1.5, 20, 20);

				for(i = 1; i <= 36; i++) {
					glPushMatrix(); {
						glColor3f(0,0,0);
						glRotatef(10*i, 0,0,1);
						glTranslatef(2.9,0,0);

						glBegin(GL_LINES); {
							//Y axis
							glVertex3f(0, 0, 0);	// intentionally extended to -150 to 150, no big deal
							glVertex3f(0, 0, 1.5);

							//X axis
							glVertex3f(0, .1, 0);
							glVertex3f(0, 0.1, 1.5);
						}
						glEnd();
					}
					glPopMatrix();
				}
			}
			glPopMatrix();
		}glPopMatrix();
		//curve 2
		glPushMatrix(); {
			glTranslatef(0,0,26);
			glScalef(.8,.8,1);
			glColor3f(.9,.9,.9);
			gluCylinder(IDquadric, 2.9, 2.9, .5, 20, 20);
			glutSolidCone(2.9,.001, 20, 20);
			
			glPushMatrix(); {
				glTranslatef(0,0,.5);
				glColor3f(.8,.8,.8);
				GLUquadricObj *quadratic;
				quadratic = gluNewQuadric();
				gluCylinder(quadratic, 2.9, 2.9, 1.5, 20, 20);
				for(i = 1; i <= 36; i++){
					glPushMatrix(); {
						glColor3f(0,0,0);
						
						glRotatef(10*i, 0,0,1);
						glTranslatef(2.9,0,0);

						glBegin(GL_LINES); {
						//Y axis
						glVertex3f(0, 0, 0);	// intentionally extended to -150 to 150, no big deal
						glVertex3f(0, 0, 1.5);

						//X axis
						glVertex3f(0, .1, 0);
						glVertex3f(0, 0.1, 1.5);
						}
						glEnd();
					}glPopMatrix();
				}
			}glPopMatrix();
		}glPopMatrix();
		//curve 3
		glPushMatrix(); {
			glTranslatef(0,0,38);
			glScalef(.6,.6,1);
			glColor3f(.7,.7,.7);
			GLUquadricObj *quadratic;
			quadratic = gluNewQuadric();
			gluCylinder(quadratic, 2.9, 2.9, .5, 20, 20);
			glutSolidCone(2.9,.001, 20, 20);
			
			glPushMatrix(); {
				glTranslatef(0,0,.5);
				glColor3f(.5,.5,.5);
				GLUquadricObj *quadratic;
				quadratic = gluNewQuadric();
				gluCylinder(quadratic, 2.9, 2.9, 1.5, 20, 20);
				for(i = 1; i <= 36; i++) {
					glPushMatrix(); {
						glColor3f(0,0,0);
						
						glRotatef(10*i, 0,0,1);
						glTranslatef(2.9,0,0);
						glBegin(GL_LINES);{
						//Y axis
						glVertex3f(0, 0, 0);	// intentionally extended to -150 to 150, no big deal
						glVertex3f(0, 0, 1.5);

						//X axis
						glVertex3f(0, .1, 0);
						glVertex3f(0, 0.1, 1.5);
						}glEnd();
					}glPopMatrix();
				}
			}glPopMatrix();
		}glPopMatrix();
		//curve end

		//gombuj start
		glPushMatrix();{
			glColor3f(.7,.7,.7);
			glTranslatef(0,0,40);
			glutSolidCone(1.5,.001,10,10);

			for(i =0; i < 8; i++){
				glPushMatrix();{
					glColor3f(.9,.9,.9);
					glRotatef(45 * (i+1), 0,0,1);
					glTranslatef(1,0,0);
					gluCylinder(IDquadric, .05, .04, 1.5, 10, 10);
					
					glPushMatrix();{
						glRotatef(90 + 22.5,0,0,1);
						glBegin(GL_TRIANGLES); //Begin triangle coordinates
							//Triangle
							glVertex3f(0, 0, 1.3);
							glVertex3f(0, 0, 2.5);
							glVertex3f(.5, 0, 2.5);
						glEnd();
					}glPopMatrix();
					glPushMatrix();{
						glRotatef(-90 - 22.5,0,0,1);
						glBegin(GL_TRIANGLES); //Begin triangle coordinates
							//Triangle
							glVertex3f(0, 0, 1.3);
							glVertex3f(0, 0, 2.5);
							glVertex3f(.5, 0, 2.5);
						glEnd();
					}glPopMatrix();
				}glPopMatrix();
			}
		}glPopMatrix();

		glPushMatrix();{
			glColor3f(.9,.9,.9);
			glTranslatef(0, 0, 42.5);
			glutSolidCone(2,1,10,10);

			glPushMatrix();{
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, gombujChoto); // here texid corresponds a bitmap image.
				gluQuadricNormals(IDquadric,GLU_SMOOTH);
				gluQuadricTexture(IDquadric, GLU_TRUE);
				//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				gluCylinder(IDquadric,2, 2, .01, 10, 10);
			glDisable(GL_TEXTURE_2D);
		}glPopMatrix();
			
		}glPopMatrix();
			
		glPushMatrix();{
			glColor3f(.6,.6,.6);
			glTranslatef(0, 0, 43.5);
			double equ[4];

			equ[0] = 0;	//0.x
			equ[1] = 0;	//0.y
			equ[2] = 1;//-1.z
			equ[3] = 1;//30

			glClipPlane(GL_CLIP_PLANE0,equ);

			//now we enable the clip plane
			glScalef(.35,.35,.7);
			glEnable(GL_CLIP_PLANE0);{
				glPushMatrix();{
					glColor3f(.9,.9,.9);
						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, gombujChoto);
						gluQuadricNormals(IDquadric,GLU_SMOOTH);
						gluQuadricTexture(IDquadric, GLU_TRUE);
						gluSphere(IDquadric,4,20,20);
				}glPopMatrix();
				//glutSolidSphere(4,20,20);
			}glDisable(GL_CLIP_PLANE0);
		}glPopMatrix();

		glPushMatrix();{
			glColor3f(.90625,.734375, .3828125);
			glTranslatef(0,0,46);
			glutSolidCone(.2,2,20,20);
		}glPopMatrix();
		//gombuj end
	}glPopMatrix();
	//pillar
}


void pillar1(){
	int i;
			
			glPushMatrix();{
				glColor3f(.5,.5,.5);
				glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, wallTexRound); // here texid corresponds a bitmap image.
					gluQuadricNormals(IDquadric,GLU_SMOOTH);
					gluQuadricTexture(IDquadric, GLU_TRUE);
					gluCylinder(IDquadric,4 ,4 ,10,8,8);
				glDisable(GL_TEXTURE_2D);
			}glPopMatrix();
			glPushMatrix();{
				glTranslatef(0,0,10);
				
				glutSolidCone(4,.001, 8, 8);
			}glPopMatrix();
			
			//pillar
			glPushMatrix();{
				glTranslatef(0,0,10);
				//glColor3f(.6,.6,.6);
				glPushMatrix();{
					glColor3f(1,1,1);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, chotoPillar); // here texid corresponds a bitmap image.
						gluQuadricNormals(IDquadric,GLU_SMOOTH);
						gluQuadricTexture(IDquadric, GLU_TRUE);
						//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
						gluCylinder(IDquadric,2.5, 1.5, 40, 50, 50);
					glDisable(GL_TEXTURE_2D);
				}glPopMatrix();
				
				//curve 3
				glPushMatrix();{
					glTranslatef(0,0,38);
					glScalef(.6,.6,1);
					glColor3f(.7,.7,.7);
					GLUquadricObj *quadratic;
					quadratic = gluNewQuadric();
					gluCylinder(quadratic, 2.9, 2.9, .5, 20, 20);
					glutSolidCone(2.9,.001, 20, 20);
					
					glPushMatrix();{
						glTranslatef(0,0,.5);
						glColor3f(.5,.5,.5);
						GLUquadricObj *quadratic;
						quadratic = gluNewQuadric();
						gluCylinder(quadratic, 2.9, 2.9, 1.5, 20, 20);
						for(i = 1; i <= 36; i++){
							glPushMatrix();{
								glColor3f(0,0,0);
								
								glRotatef(10*i, 0,0,1);
								glTranslatef(2.9,0,0);
								glBegin(GL_LINES);{
									//Y axis
									glVertex3f(0, 0, 0);	// intentionally extended to -150 to 150, no big deal
									glVertex3f(0, 0, 1.5);

									//X axis
									glVertex3f(0, .1, 0);
									glVertex3f(0, 0.1, 1.5);
								}glEnd();
							}glPopMatrix();
						}
					}glPopMatrix();
				}glPopMatrix();
				//curve end

				//gombuj start
				glPushMatrix();{
					glColor3f(.7,.7,.7);
					glTranslatef(0,0,40);
					glutSolidCone(1.5,.001,10,10);

					for(i =0; i < 8; i++){
						glPushMatrix();{
							glColor3f(.9,.9,.9);
							glRotatef(45 * (i+1), 0,0,1);
							glTranslatef(1,0,0);
							gluCylinder(IDquadric, .05, .04, 1.5, 10, 10);
							
							glPushMatrix();{
								glRotatef(90 + 22.5,0,0,1);
								glBegin(GL_TRIANGLES); //Begin triangle coordinates
									//Triangle
									glVertex3f(0, 0, 1.3);
									glVertex3f(0, 0, 2.5);
									glVertex3f(.5, 0, 2.5);
								glEnd();
							}glPopMatrix();
							glPushMatrix();{
								glRotatef(-90 - 22.5,0,0,1);
								glBegin(GL_TRIANGLES); //Begin triangle coordinates
									//Triangle
									glVertex3f(0, 0, 1.3);
									glVertex3f(0, 0, 2.5);
									glVertex3f(.5, 0, 2.5);
								glEnd();
							}glPopMatrix();
						}glPopMatrix();
					}
				}glPopMatrix();

				glPushMatrix();{
					glColor3f(.9,.9,.9);
					glTranslatef(0, 0, 42.5);
					glutSolidCone(2,1,10,10);

					glPushMatrix();{
					glColor3f(1,1,1);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, gombujChoto); // here texid corresponds a bitmap image.
						gluQuadricNormals(IDquadric,GLU_SMOOTH);
						gluQuadricTexture(IDquadric, GLU_TRUE);
						//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
						gluCylinder(IDquadric,2, 2, .01, 10, 10);
					glDisable(GL_TEXTURE_2D);
				}glPopMatrix();
					
				}glPopMatrix();
					
				glPushMatrix();{
					glColor3f(.6,.6,.6);
					glTranslatef(0, 0, 43.5);
					double equ[4];

					equ[0] = 0;	//0.x
					equ[1] = 0;	//0.y
					equ[2] = 1;//-1.z
					equ[3] = 1;//30

					glClipPlane(GL_CLIP_PLANE0,equ);

					//now we enable the clip plane
					glScalef(.35,.35,.7);
					glEnable(GL_CLIP_PLANE0);{
						glPushMatrix();{
							glColor3f(.9,.9,.9);
								glEnable(GL_TEXTURE_2D);
								glBindTexture(GL_TEXTURE_2D, gombujChoto);
								gluQuadricNormals(IDquadric,GLU_SMOOTH);
								gluQuadricTexture(IDquadric, GLU_TRUE);
								gluSphere(IDquadric,4,20,20);
						}glPopMatrix();
						//glutSolidSphere(4,20,20);
					}glDisable(GL_CLIP_PLANE0);
				}glPopMatrix();

				glPushMatrix();{
					glColor3f(.90625,.734375, .3828125);
					glTranslatef(0,0,46);
					glutSolidCone(.2,2,20,20);
				}glPopMatrix();
				//gombuj end
			}glPopMatrix();
			//pillar
	}

void gombuj(){
		int i;
		glPushMatrix();{
					glColor3f(.7,.7,.7);
					glTranslatef(0,0,40);
					//glutSolidCone(1.5,.001,10,10);

					for(i =0; i < 8; i++){
						glPushMatrix();{
							glColor3f(.9,.9,.9);
							glRotatef(45 * (i+1), 0,0,1);
							glTranslatef(1,0,0);
							GLUquadricObj *quadratic;
							quadratic = gluNewQuadric();
							gluCylinder(quadratic, .05, .04, 1.5, 10, 10);
							
							glPushMatrix();{
								glRotatef(90 + 22.5,0,0,1);
								glBegin(GL_TRIANGLES); //Begin triangle coordinates
									//Triangle
									glVertex3f(0, 0, 1.3);
									glVertex3f(0, 0, 2.5);
									glVertex3f(.5, 0, 2.5);
								glEnd();
							}glPopMatrix();
							glPushMatrix();{
								glRotatef(-90 - 22.5,0,0,1);
								glBegin(GL_TRIANGLES); //Begin triangle coordinates
									//Triangle
									glVertex3f(0, 0, 1.3);
									glVertex3f(0, 0, 2.5);
									glVertex3f(.5, 0, 2.5);
								glEnd();
							}glPopMatrix();
						}glPopMatrix();
					}
				}glPopMatrix();

				glPushMatrix();{
					glColor3f(.9,.9,.9);
					glTranslatef(0, 0, 42.5);
					glutSolidCone(2,1,10,10);
				}glPopMatrix();
					
				glPushMatrix();{
					glColor3f(.75,.75,.75);
					glTranslatef(0, 0, 43.5);
					double equ[4];

					equ[0] = 0;	//0.x
					equ[1] = 0;	//0.y
					equ[2] = 1;//-1.z
					equ[3] = 1;//30

					glClipPlane(GL_CLIP_PLANE0,equ);

					//now we enable the clip plane
					glScalef(.35,.35,.7);
					glEnable(GL_CLIP_PLANE0);{
						glPushMatrix();{
							glColor3f(1,1,1);
								glEnable(GL_TEXTURE_2D);
								glBindTexture(GL_TEXTURE_2D, gombujChoto);
								gluQuadricNormals(IDquadric,GLU_SMOOTH);
								gluQuadricTexture(IDquadric, GLU_TRUE);
								gluSphere(IDquadric,4,20,20);
						}glPopMatrix();
						//glutSolidSphere(4,20,20);
					}glDisable(GL_CLIP_PLANE0);
				}glPopMatrix();

				glPushMatrix();{
					glColor3f(.90625,.734375, .3828125);
					glTranslatef(0,0,46);
					glutSolidCone(.2,2,20,20);
				}glPopMatrix();
	}

void tile1(){
	glPushMatrix();{
			glColor3f(0, 0, 0);
			glLineWidth(1.0);
			glBegin(GL_LINES);{
				glVertex3f(-1, .01, 2);	// intentionally extended to -150 to 150, no big deal
				glVertex3f(1,  .01, 2);

				glVertex3f(-1, .01, -5.9);
				glVertex3f( 1, .01, -5.9);

				glVertex3f(-1,.01, -5.9);
				glVertex3f(-1,.01, 2);

				glVertex3f(1,.01, -5.9);
				glVertex3f(1,.01, 2);

				glVertex3f(-.8, .01, .5);	// intentionally extended to -150 to 150, no big deal
				glVertex3f(-.6,  .01, .8);
				
				glVertex3f(.8, .01, .5);	// intentionally extended to -150 to 150, no big deal
				glVertex3f(.6,  .01, .8);

				glVertex3f(-.6,.01, .8);
				glVertex3f(0,.01, 1.6);

				glVertex3f(.6,.01, .8);
				glVertex3f(0,.01, 1.6);
				
				glVertex3f(-.8, .01, -5.6);
				glVertex3f( .8, .01, -5.6);

				glVertex3f(-.8,.01, -5.6);
				glVertex3f(-.8,.01, .5);

				glVertex3f(.8,.01, -5.6);
				glVertex3f(.8,.01, .5);
			}glEnd();
		}glPopMatrix();

		glPushMatrix();{
			glColor3f(0, 0, 0);
			glLineWidth(3.0);
			glBegin(GL_LINES);{
				glVertex3f(-1.25, .01, 4);
				glVertex3f(1.25, .01, 4);

				glVertex3f(-1, .01, 3.6);	// intentionally extended to -150 to 150, no big deal
				glVertex3f(1,  .01, 3.6);

				glVertex3f(-1, .01, 2.4);
				glVertex3f( 1, .01, 2.4);

				glVertex3f(-1,.01, 2.4);
				glVertex3f(-1,.01, 3.6);

				glVertex3f(1,.01, 2.4);
				glVertex3f(1,.01, 3.6);

				glVertex3f(-1, .01, 5.6);	// intentionally extended to -150 to 150, no big deal
				glVertex3f(1,  .01, 5.6);

				glVertex3f(-1, .01, 4.4);
				glVertex3f( 1, .01, 4.4);

				glVertex3f(-1,.01, 4.4);
				glVertex3f(-1,.01, 5.6);

				glVertex3f(1,.01, 4.4);
				glVertex3f(1,.01, 5.6);


			}glEnd();
		}glPopMatrix();

		glColor3f(0.7,0.7,0.7);
		glScalef(2.5,.01, 12);
		glutSolidCube(1);
}



void entranceBig(){
	glColor3f(.7,.7,.7);
		glPushMatrix();{
			glTranslatef(7,0,0);
			glRotatef(90,0,0,1);
			glScalef(1,.1, 4.3);
			glutSolidCube(6);
		}glPopMatrix();

		glPushMatrix();{
			glTranslatef(-7,0,0);
			glRotatef(90,0,0,1);
			glScalef(1,.1, 4.3);
			glutSolidCube(6);
		}glPopMatrix();

		glPushMatrix();{
			glTranslatef(0,0,-11.5);
			glRotatef(180,0,0,1);
			glScalef(2,.92, .1);
			glutSolidCube(7);
		}glPopMatrix();

		glPushMatrix();{
			glTranslatef(0,0,11.5);
			glRotatef(180,0,0,1);
			glScalef(2,.92, .1);
			glutSolidCube(6);
		}glPopMatrix();

		glPushMatrix();{
			glColor3f(.8,.8,.8);
			glTranslatef(0,2.9,0);
			glRotatef(180,0,0,1);
			glScalef(2,.1, 3.4);
			glutSolidCube(7);
		}glPopMatrix();

		glPushMatrix();{

			glPushMatrix();{
				glColor3f(1,1,1);
				glTranslatef(0,0,2.8);
				glRotatef(180,0,0,1);
				glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D,front1);
					glNormal3f(0.0,1.0,0.0);
					glBegin(GL_POLYGON);
						glTexCoord2f(0,0); glVertex3f(-6.8,0,-15);
						glTexCoord2f(0,1); glVertex3f(-6.8,0,-3);
						glTexCoord2f(1,1); glVertex3f(6.8,0,-3);
						glTexCoord2f(1,0); glVertex3f(6.8,0,-15);
					glEnd();
				glDisable(GL_TEXTURE_2D);
			}glPopMatrix();

			glPushMatrix();{
				glColor3f(1,1,1);
				glTranslatef(0,0,2.8);
				glRotatef(180,0,0,1);
				glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D,front2);
					glNormal3f(0.0,1.0,0.0);
					glBegin(GL_POLYGON);
						glTexCoord2f(0,0); glVertex3f(-7,0,-3);
						glTexCoord2f(0,1); glVertex3f(-7,0,10);
						glTexCoord2f(1,1); glVertex3f(7,0,10);
						glTexCoord2f(1,0); glVertex3f(7,0,-3);
					glEnd();
				glDisable(GL_TEXTURE_2D);
			}glPopMatrix();
		}glPopMatrix();

		glPushMatrix();{
			glColor3f(1,1,1);
			glRotatef(180, 0,0,1);
			glTranslatef(0,3,3);

			glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,front3);
				glNormal3f(0.0,1.0,0.0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(-6.7,0,2);
					glTexCoord2f(1,0); glVertex3f(-6.7,0,8.5);
					glTexCoord2f(1,1); glVertex3f(2,0,8.5);
				glEnd();

				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(-6.7,0,0);
					glTexCoord2f(1,0); glVertex3f(-6.7,0,8.5);
					glTexCoord2f(1,1); glVertex3f(-2,0,8.5);
				glEnd();
				
			glDisable(GL_TEXTURE_2D);

			//glRotatef(180, 0,0,1);
			glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,front3);
				glNormal3f(0.0,1.0,0.0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(6.7,0,2);
					glTexCoord2f(1,0); glVertex3f(6.7,0,8.5);
					glTexCoord2f(1,1); glVertex3f(-2,0,8.5);
				glEnd();

				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(6.7,0,0);
					glTexCoord2f(1,0); glVertex3f(6.7,0,8.5);
					glTexCoord2f(1,1); glVertex3f(2,0,8.5);
				glEnd();
			glDisable(GL_TEXTURE_2D);

			
			glRotatef(180, 0,0,1);
			glColor3f(.7,.7,.7);
			glBegin(GL_TRIANGLES); //Begin triangle coordinates
				//Triangle
				glVertex3f(6.7, 0, 0);
				glVertex3f(2,0, 8.5);
				glVertex3f(6.7,0,8.5);
				
				glVertex3f(6.7, 0, 2);
				glVertex3f(-2, 0, 8.5);
				glVertex3f(6.7,0,8.5);

			glEnd(); 
			
			glBegin(GL_QUADS); 
				glVertex3f(6.7, 0, 0);
				glVertex3f(6.7,5.9, 0);
				glVertex3f(2,5.9, 8.5);
				glVertex3f(2,0, 8.5);
				
				glVertex3f(6.7, 0, 2);				
				glVertex3f(6.7,5.9,2);
				glVertex3f(-2, 5.9, 8.5);
				glVertex3f(-2, 0, 8.5);
			glEnd();
		}glPopMatrix();

		glPushMatrix();{
			//glColor3f(.7,.7,.7);
			glTranslatef(0,-3,3);
			glRotatef(180,0,0,1);
			glBegin(GL_TRIANGLES); //Begin triangle coordinates
				//Triangle
				glVertex3f(6.7, 0, 0);
				glVertex3f(2,0, 8.5);
				glVertex3f(6.7,0,8.5);
				
				glVertex3f(6.7, 0, 2);
				glVertex3f(-2, 0, 8.5);
				glVertex3f(6.7,0,8.5);
			glEnd(); 

			glBegin(GL_QUADS); 
				glVertex3f(6.7, 0, 0);
				glVertex3f(6.7,-5.9, 0);
				glVertex3f(2,-5.9, 8.5);
				glVertex3f(2,0, 8.5);
				
				glVertex3f(6.7, 0, 2);				
				glVertex3f(6.7,-5.9,2);
				glVertex3f(-2, -5.9, 8.5);
				glVertex3f(-2, 0, 8.5);
			glEnd();
		}glPopMatrix();
}

void entranceSmaller(){
	glPushMatrix();{
		glTranslatef(0,0,-6);
		glScalef(1,1,.5);
		entranceBig();
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(0,0,6);
		glScalef(1,1,.5);
		entranceBig();
	}glPopMatrix();
	
	glPushMatrix();{
		glColor3f(1,1,1);
		glTranslatef(0, -3.02, 12);
		glRotatef(180,0,0,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,frontTop1);
			glNormal3f(0.0,1.0,0.0);
			glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(-8,0,0);
				glTexCoord2f(0,1); glVertex3f(-8,0,4);
				glTexCoord2f(1,1); glVertex3f(8,0,4);
				glTexCoord2f(1,0); glVertex3f(8,0,0);
			glEnd();
		glDisable(GL_TEXTURE_2D);
		}glPopMatrix();
	
	glPushMatrix();{
		glColor3f(.6,.6,.6);
		glTranslatef(0, 0, 12.5);
		glRotatef(180,0,0,1);
		glScalef(3, 1.2, .2);
		glutSolidCube(5);
	}glPopMatrix();

	glPushMatrix();{
		glColor3f(.3,.3,.3);
		glTranslatef(0, 0, 13.5);
		glRotatef(180,0,0,1);
		glScalef(3.1, 1.2, .2);
		glutSolidCube(5);
	}glPopMatrix();

	glPushMatrix();{
		glColor3f(.6,.6,.6);
		glTranslatef(0, 0, 15);
		glRotatef(180,0,0,1);
		glScalef(3, 1.2, .4);
		glutSolidCube(5);
	}glPopMatrix();
	
	glPushMatrix();{
		glColor3f(.9,.9,.9);
		glTranslatef(0, -2.99, 16.02);
		glRotatef(180,0,0,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,gombujChoto);
			glNormal3f(0.0,0.0,-1.0);
			glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(-8,0,0);
				glTexCoord2f(0,1); glVertex3f(-8,6,0);
				glTexCoord2f(1,1); glVertex3f(8,6,0);
				glTexCoord2f(1,0); glVertex3f(8,0,0);
			glEnd();
		glDisable(GL_TEXTURE_2D);
	}glPopMatrix();

}


void front(){
	entranceBig();
		
		glPushMatrix();{
			glColor3f(.6,.6,.6);
			glTranslatef(9.5, 0, 2);
			glScalef(1, 1.2, 6);
			glutSolidCube(5);

		}glPopMatrix();

		glPushMatrix();{
			glColor3f(1,1,1);
			glTranslatef(12, -4, -21);
			glScalef(.3,.3,.9);
			pillar1();
		}glPopMatrix();

		glPushMatrix();{
			glColor3f(1,1,1);
			glTranslatef(-12, -4, -21);
			glScalef(.3,.3,.9);
			pillar1();
		}glPopMatrix();

		glPushMatrix();{
			glColor3f(.6,.6,.6);
			glTranslatef(-9.5, 0, 2);
			glScalef(1, 1.2, 6);
			glutSolidCube(5);
		}glPopMatrix();


		glPushMatrix();{
			glColor3f(.6,.6,.6);
			glTranslatef(0, 0, 14.5);
			glScalef(3, 1.2, 1);
			glutSolidCube(5);
		}glPopMatrix();

		glPushMatrix();{
			glColor3f(1,1,1);
			glTranslatef(0, -3.01, 11);
			glRotatef(180, 0, 0, 1);
			glPushMatrix();{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,frontTop);
				glNormal3f(0.0,1.0,0.0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(-11,0,0);
					glTexCoord2f(0,1); glVertex3f(-11,0,9);
					glTexCoord2f(1,1); glVertex3f(11,0,9);
					glTexCoord2f(1,0); glVertex3f(11,0,0);
				glEnd();
			glDisable(GL_TEXTURE_2D);
			}glPopMatrix();
		}glPopMatrix();

		glPushMatrix();{
			glColor3f(1,1,1);
			glTranslatef(0, -3.01, -13);
			glRotatef(180, 0, 0, 1);
			glPushMatrix();{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,frontSide);
				glNormal3f(0.0,1.0,0.0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(7,0,0);
					glTexCoord2f(0,1); glVertex3f(7,0,25);
					glTexCoord2f(1,1); glVertex3f(11,0,25);
					glTexCoord2f(1,0); glVertex3f(11,0,0);
				glEnd();

				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(-7,0,0);
					glTexCoord2f(0,1); glVertex3f(-7,0,25);
					glTexCoord2f(1,1); glVertex3f(-11,0,25);
					glTexCoord2f(1,0); glVertex3f(-11,0,0);
				glEnd();
			glDisable(GL_TEXTURE_2D);
			}glPopMatrix();
		}glPopMatrix();

		glPushMatrix();{
			glColor3f(1,1,1);
			glTranslatef(0, -3.01, -13);
			glRotatef(180, 0, 0, 1);
			glPushMatrix();{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,frontSide1);
				glNormal3f(0.0,1.0,0.0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(11,0,0);
					glTexCoord2f(0,1); glVertex3f(11,0,33);
					glTexCoord2f(1,1); glVertex3f(12,0,33);
					glTexCoord2f(1,0); glVertex3f(12,0,0);
				glEnd();

				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(-11,0,0);
					glTexCoord2f(0,1); glVertex3f(-11,0,33);
					glTexCoord2f(1,1); glVertex3f(-12,0,33);
					glTexCoord2f(1,0); glVertex3f(-12,0,0);
				glEnd();
			glDisable(GL_TEXTURE_2D);
			}glPopMatrix();
		}glPopMatrix();

		glPushMatrix();{
			glColor3f(.9,.9,.9);
			glTranslatef(0, -2.99, 20.02);
			//glRotatef(180, 0, 0, 1);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,gombujChoto);
				glNormal3f(0.0,0.0,-1.0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(-12,0,0);
					glTexCoord2f(0,1); glVertex3f(-12,6,0);
					glTexCoord2f(3,1); glVertex3f(12,6,0);
					glTexCoord2f(3,0); glVertex3f(12,0,0);
				glEnd();
			glDisable(GL_TEXTURE_2D);
			}glPopMatrix();

		glPushMatrix();{
			glColor3f(.3,.3,.3);
			glTranslatef(0, 0, 17.5);
			//glRotatef(180, 0, 0, 1);
			glScalef(5, 1.2, .2);
			glutSolidCube(5);
		}glPopMatrix();

		glPushMatrix();{
			glColor3f(.6,.6,.6);
			glTranslatef(0, 0, 19);
			//glRotatef(180, 0, 0, 1);
			glScalef(4.8, 1.2, .4);
			glutSolidCube(5);
		}glPopMatrix();

		glPushMatrix();{
			glTranslatef(16.4, -2.5, -1.5);
			glScalef(.6,.2,.9);
			entranceSmaller();
		}glPopMatrix();

		glPushMatrix();{
			glColor3f(1,1,1);
			glTranslatef(21, -4, -18);
			glScalef(.2,.2,.65);
			pillar1();
		}glPopMatrix();

		glPushMatrix();{
			glColor3f(1,1,1);
			glTranslatef(-21, -4, -18);
			glScalef(.2,.2,.65);
			pillar1();
		}glPopMatrix();

		glPushMatrix();{
			glTranslatef(-16.4, -2.5, -1.5);
			glScalef(.6,.2,.9);
			entranceSmaller();
		}glPopMatrix();
}



void Piller_one()
{
	glPushMatrix();{

		GLUquadricObj *quadratic;
		quadratic = gluNewQuadric();
		glColor3f(0.5,0.5,0.5);
		//glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		gluCylinder(quadratic,20,20,45,8,8);
	}glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0,0,45);
		GLUquadricObj *quadratic;
		quadratic = gluNewQuadric();
		glColor3f(0,0.5,0.5);
		//glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		gluCylinder(quadratic,18,18,35,8,8);
	}glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0,0,80);
		GLUquadricObj *quadratic;
		quadratic = gluNewQuadric();
		glColor3f(1,0.5,0.5);
		//glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		gluCylinder(quadratic,25,25,10,8,8);
	}glPopMatrix();

	glPushMatrix();
	{
		glBegin(GL_POLYGON);
		{
			glColor3f(1,1,1);
			glVertex3f(0,0,80);
			glVertex3f(-3,25,80);
			glVertex3f(20,23,80);
			

		}
		glEnd();
	}glPopMatrix();
}

void Draw_Sphare()
{
	double equ[4];
	int i = 0;
	equ[0] = 0;	//0.x
	equ[1] = 0;	//0.y
	equ[2] = -1;//-1.z
	equ[3] = 30;//30

	glClipPlane(GL_CLIP_PLANE0,equ);

	//now we enable the clip plane

	glEnable(GL_CLIP_PLANE0);{
	}glDisable(GL_CLIP_PLANE0);

	//drawing begins
	glColor3f(.8,0.8,0.8);
	glTranslatef(0,0,5);

	glPushMatrix();{
		glScalef(10, 10, 1);
		glutSolidCube(10);
	}glPopMatrix();

	
		for(i = 0; i < 37; i++)
		{
			glPushMatrix();{
				glTranslatef(-45 + (i*2.5), 50.01, 1);
				tile1();
			}glPopMatrix();
		}

		for(i = 0; i < 37; i++){
			glPushMatrix();{
				glTranslatef(50.1, -45 + (i*2.5), 1);
				glRotatef(270,0,0,1);
				tile1();
			}glPopMatrix();
		}

		for(i = 0; i < 37; i++){
			glPushMatrix();{
				glTranslatef(-45 + (i*2.5), -50.01, 1);
				glRotatef(180,0,0,1);
				tile1();
			}glPopMatrix();
		}
		
		for(i = 0; i < 37; i++){
			glPushMatrix();{
				glTranslatef(-50.1, -45 + (i*2.5), 1);
				glRotatef(90,0,0,1);
				tile1();
			}glPopMatrix();
		}
	
}
void Roof_Dumb()
{
	double equ[4];

	equ[0] = 0;	//0.x
	equ[1] = 0;	//0.y
	equ[2] = 1;//-1.z
	equ[3] = -70;//30

	glClipPlane(GL_CLIP_PLANE0,equ);

	//now we enable the clip plane

	glEnable(GL_CLIP_PLANE0);
	{
		glColor3f(1,1,1);	//blue

		glPushMatrix();
		{
			glTranslatef(0,0,95);
			glutSolidSphere(50, 20, 20);	//radius, slices(XY), stacks(Z) -- like earth
		}
		glPopMatrix();
	}
	glDisable(GL_CLIP_PLANE0);

	glPushMatrix();
	{
		glTranslatef(0,0,45);
		GLUquadricObj *quadratic;
		quadratic = gluNewQuadric();
		glColor3f(0.5,0.5,0.5);
		//glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		gluCylinder(quadratic,42,42,25,16,16);
	}glPopMatrix();

	glPushMatrix();
	{
		double equ[4];

	equ[0] = 0;	//0.x
	equ[1] = 0;	//0.y
	equ[2] = -1;//-1.z
	equ[3] = 160;//30

	glClipPlane(GL_CLIP_PLANE0,equ);

	//now we enable the clip plane

	glEnable(GL_CLIP_PLANE0);
	{
		glPushMatrix();
		{
			glColor3f(0,0.3,0.8);
			glTranslatef(0,0,135);
			glutSolidCone(15,30,20,20);
			//GLUquadricObj *quadObj = gluNewQuadric();
			//gluCylinder(quadObj, 35, 15, 10, 10, 10);
		}glPopMatrix();
	}
	glDisable(GL_CLIP_PLANE0);
		
	}glPopMatrix();

	glPushMatrix();
		{
			glColor3f(1,1,1);
			glTranslatef(0,0,163);
			glutSolidSphere(5, 20, 20);	//radius, slices(XY), stacks(Z) -- like earth
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(0,0.3,0.8);
			glTranslatef(0,0,165);
			glutSolidCone(3,10,20,20);
			//GLUquadricObj *quadObj = gluNewQuadric();
			//gluCylinder(quadObj, 35, 15, 10, 10, 10);
		}glPopMatrix();

}
void Red_Building_Door()
{
	glPushMatrix();
	{
		glColor3f(1,1,1);
		glBegin(GL_POLYGON);
		{
			glVertex3f(35,20,0);
			glVertex3f(35,-20,0);
			glVertex3f(35,-20,80);
			glVertex3f(35,20,80);
			

		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(0,.8,1);
		glBegin(GL_POLYGON);
		{
			glVertex3f(35,20,2);
			glVertex3f(50,20,2);
			glVertex3f(50,-20,2);
			glVertex3f(35,-20,2);
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(0,.3,1);
		glBegin(GL_POLYGON);
		{
			glVertex3f(35,20,2);
			glVertex3f(35,20,80);
			glVertex3f(50,20,80);
			glVertex3f(50,20,2);
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(0,.3,1);
		glBegin(GL_POLYGON);
		{
			glVertex3f(35,-20,2);
			glVertex3f(35,-20,80);
			glVertex3f(50,-20,80);
			glVertex3f(50,-20,2);
		}
		glEnd();
	}glPopMatrix();
	glPushMatrix();
	{
		glColor3f(1,.1,1);
		glBegin(GL_POLYGON);
		{
			glVertex3f(50,-20,2);
			glVertex3f(50,-20,105);
			glVertex3f(50,-40,105);
			glVertex3f(50,-40,2);
		}
		glEnd();
	}glPopMatrix();
	glPushMatrix();
	{
		glColor3f(1,.1,1);
		glBegin(GL_POLYGON);
		{
			glVertex3f(50,20,2);
			glVertex3f(50,20,105);
			glVertex3f(50,40,105);
			glVertex3f(50,40,2);
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.1,.1,1);
		glBegin(GL_POLYGON);
		{
			glVertex3f(50,20,50);
			glVertex3f(50,20,75);
			glVertex3f(50,0,75);
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.1,.1,1);
		glBegin(GL_POLYGON);
		{
			glVertex3f(50,-20,50);
			glVertex3f(50,-20,75);
			glVertex3f(50,0,75);
			
		}
		glEnd();
	}glPopMatrix();
	glPushMatrix();
	{
		glColor3f(.5,.8,.9);
		glBegin(GL_POLYGON);
		{
			glVertex3f(50,-20,75);
			glVertex3f(50,-20,85);
			glVertex3f(50,20,85);
			glVertex3f(50,20,75);
			
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.5,.5,.5);
		glBegin(GL_POLYGON);
		{
			glVertex3f(50,-20,105);
			glVertex3f(50,-20,85);
			glVertex3f(50,20,85);
			glVertex3f(50,20,105);
			
			
		}
		glEnd();
	}glPopMatrix();
	glPushMatrix();
	{
		glColor3f(.5,.5,.5);
		glBegin(GL_POLYGON);
		{
			glVertex3f(50,-40,105);
			glVertex3f(40,-40,105);
			glVertex3f(40,40,105);
			glVertex3f(50,40,105);
			
			
		}
		glEnd();
	}glPopMatrix();
	glPushMatrix();
	{
		glColor3f(.5,.5,.5);
		glBegin(GL_POLYGON);
		{
			glVertex3f(40,-40,105);
			glVertex3f(40,-40,100);
			glVertex3f(40,40,100);
			glVertex3f(40,40,105);
			
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.5,.5,.5);
		glBegin(GL_POLYGON);
		{
			glVertex3f(40,-40,105);
			glVertex3f(50,-40,105);
			glVertex3f(50,-40,100);
			glVertex3f(40,-40,100);
			
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.5,.5,.5);
		glBegin(GL_POLYGON);
		{
			glVertex3f(40,40,105);
			glVertex3f(50,40,105);
			glVertex3f(50,40,100);
			glVertex3f(40,40,100);
			
			
		}
		glEnd();
	}glPopMatrix();
}

void Door_Side_wall()
{
	
	glPushMatrix();
	{
		glColor3f(1,1,1);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,20,0);
			glVertex3f(0,-20,0);
			glVertex3f(0,-20,40);
			glVertex3f(0,20,40);
			
			
		}
		glEnd();
	}glPopMatrix();
	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(5,20,0);
			glVertex3f(5,-20,0);
			glVertex3f(5,-20,5);
			glVertex3f(5,20,5);
			
			
		}
		glEnd();
	}glPopMatrix();
	
	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(5,20,0);
			glVertex3f(0,20,0);
			glVertex3f(0,20,5);
			glVertex3f(5,20,5);
			
			
		}
		glEnd();
	}glPopMatrix();
	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(5,-20,0);
			glVertex3f(0,-20,0);
			glVertex3f(0,-20,5);
			glVertex3f(5,-20,5);
			
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,20,0);
			glVertex3f(0,-20,0);
			glVertex3f(5,-20,0);
			glVertex3f(5,20,0);
			
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,20,5);
			glVertex3f(0,-20,5);
			glVertex3f(5,-20,5);
			glVertex3f(5,20,5);
			
			
		}
		glEnd();
	}glPopMatrix();

	//////////////////////////
	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(3,20,35);
			glVertex3f(3,-20,35);
			glVertex3f(3,-20,40);
			glVertex3f(3,20,40);
			
			
		}
		glEnd();
	}glPopMatrix();
	
	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(3,20,35);
			glVertex3f(0,20,35);
			glVertex3f(0,20,40);
			glVertex3f(3,20,40);
			
			
		}
		glEnd();
	}glPopMatrix();
	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(3,-20,35);
			glVertex3f(0,-20,35);
			glVertex3f(0,-20,40);
			glVertex3f(3,-20,40);
			
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,20,35);
			glVertex3f(0,-20,35);
			glVertex3f(3,-20,35);
			glVertex3f(3,20,35);
			
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,20,40);
			glVertex3f(0,-20,40);
			glVertex3f(3,-20,40);
			glVertex3f(3,20,40);
			
			
		}
		glEnd();
	}glPopMatrix();
	/////////////////////////

	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,20,5);
			glVertex3f(3,20,5);
			glVertex3f(3,20,35);
			glVertex3f(0,20,35);
			
			
		}
		glEnd();
	}glPopMatrix();
	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,-20,5);
			glVertex3f(3,-20,5);
			glVertex3f(3,-20,35);
			glVertex3f(0,-20,35);
			
			
		}
		glEnd();
	}glPopMatrix();
	
	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,18,5);
			glVertex3f(3,18,5);
			glVertex3f(3,18,35);
			glVertex3f(0,18,35);
			
			
		}
		glEnd();
	}glPopMatrix();
	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,-18,5);
			glVertex3f(3,-18,5);
			glVertex3f(3,-18,35);
			glVertex3f(0,-18,35);
			
			
		}
		glEnd();
	}glPopMatrix();
	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(3,18,5);
			glVertex3f(3,20,5);
			glVertex3f(3,20,35);
			glVertex3f(3,18,35);
			
			
		}
		glEnd();
	}glPopMatrix();
	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(3,-18,5);
			glVertex3f(3,-20,5);
			glVertex3f(3,-20,35);
			glVertex3f(3,-18,35);
			
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(3,-18,5);
			glVertex3f(3,-20,5);
			glVertex3f(3,-20,35);
			glVertex3f(3,-18,35);
			
			
		}
		glEnd();
	}glPopMatrix();

	/////////////////////////////

	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,12,5);
			glVertex3f(3,12,5);
			glVertex3f(3,12,28);
			glVertex3f(0,12,28);
			
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,10,5);
			glVertex3f(3,10,5);
			glVertex3f(3,10,28);
			glVertex3f(0,10,28);
			
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(3,10,5);
			glVertex3f(3,12,5);
			glVertex3f(3,12,28);
			glVertex3f(3,10,28);
			
			
		}
		glEnd();
	}glPopMatrix();
	/////////////////////

	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,-12,5);
			glVertex3f(3,-12,5);
			glVertex3f(3,-12,28);
			glVertex3f(0,-12,28);
			
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,-10,5);
			glVertex3f(3,-10,5);
			glVertex3f(3,-10,28);
			glVertex3f(0,-10,28);
			
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(3,-10,5);
			glVertex3f(3,-12,5);
			glVertex3f(3,-12,28);
			glVertex3f(3,-10,28);
			
			
		}
		glEnd();
	}glPopMatrix();

	///////////////////////
	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,-12,28);
			glVertex3f(0,12,28);
			glVertex3f(3,12,28);
			glVertex3f(3,-12,28);
			
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,-12,31);
			glVertex3f(0,12,31);
			glVertex3f(3,12,31);
			glVertex3f(3,-12,31);
			
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(3,-12,31);
			glVertex3f(3,12,31);
			glVertex3f(3,12,28);
			glVertex3f(3,-12,28);
			
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(3,-12,31);
			glVertex3f(0,-12,31);
			glVertex3f(0,-12,28);
			glVertex3f(3,-12,28);
			
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.4,.2,.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(3,12,31);
			glVertex3f(0,12,31);
			glVertex3f(0,12,28);
			glVertex3f(3,12,28);
			
			
		}
		glEnd();
	}glPopMatrix();
	/////////////////////////
	glPushMatrix();
	{
		glColor3f(.3,.2,.3);
		glBegin(GL_POLYGON);
		{
			
			glVertex3f(3,-1,26);
			glVertex3f(3,2,26);
			glVertex3f(3,9,23);
			glVertex3f(3,9,21);
			
		}
		glEnd();
	}glPopMatrix();
	glPushMatrix();
	{
		glColor3f(.3,.2,.3);
		glBegin(GL_POLYGON);
		{
			
			glVertex3f(3,1,26);
			glVertex3f(3,-2,26);
			glVertex3f(3,-9,23);
			glVertex3f(3,-9,21);
			
		}
		glEnd();
	}glPopMatrix();

	///////////////////

	glPushMatrix();
	{
		glColor3f(.3,.2,.3);
		glBegin(GL_POLYGON);
		{
			
			glVertex3f(3,-1,19);
			glVertex3f(3,1,19);
			glVertex3f(3,6,15);
			glVertex3f(3,6,13);
			
		}
		glEnd();
	}glPopMatrix();
	glPushMatrix();
	{
		glColor3f(.3,.2,.3);
		glBegin(GL_POLYGON);
		{
			
			glVertex3f(3,1,19);
			glVertex3f(3,-1,19);
			glVertex3f(3,-6,15);
			glVertex3f(3,-6,13);
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.3,.2,.3);
		glBegin(GL_POLYGON);
		{
			
			glVertex3f(3,5,3);
			glVertex3f(3,6,3);
			glVertex3f(3,6,15);
			glVertex3f(3,5,15);
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.3,.2,.3);
		glBegin(GL_POLYGON);
		{
			
			glVertex3f(3,-5,3);
			glVertex3f(3,-6,3);
			glVertex3f(3,-6,15);
			glVertex3f(3,-5,15);
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.3,.2,.3);
		glBegin(GL_POLYGON);
		{
			
			glVertex3f(3,-8,3);
			glVertex3f(3,-9,3);
			glVertex3f(3,-9,23);
			glVertex3f(3,-8,23);
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.3,.2,.3);
		glBegin(GL_POLYGON);
		{
			
			glVertex3f(0,-8,3);
			glVertex3f(3,-8,3);
			glVertex3f(3,-8,23);
			glVertex3f(0,-8,23);
			
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(.3,.2,.3);
		glBegin(GL_POLYGON);
		{
			
			glVertex3f(3,8,3);
			glVertex3f(3,9,3);
			glVertex3f(3,9,23);
			glVertex3f(3,8,23);
			
		}
		glEnd();
	}glPopMatrix();
}

void Door_Side_Door()
{
	glPushMatrix();
	{
		glColor3f(0.6,0.6,0.6);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,16,0);
			glVertex3f(0,-16,0);
			glVertex3f(0,-16,40);
			glVertex3f(0,16,40);
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(1,.14,1);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,16,0);
			glVertex3f(15,16,0);
			glVertex3f(15,-16,0);
			glVertex3f(0,-16,0);
			

		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(1,.14,1);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,16,0);
			glVertex3f(0,16,40);
			glVertex3f(15,16,40);
			glVertex3f(15,16,0);
			

		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(1,.14,1);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0,-16,0);
			glVertex3f(0,-16,40);
			glVertex3f(15,-16,40);
			glVertex3f(15,-16,0);
			

		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(0.5,.5,.6);
		glBegin(GL_POLYGON);
		{
			glVertex3f(15,-16,0);
			glVertex3f(15,-16,45);
			glVertex3f(15,-19,45);
			glVertex3f(15,-19,0);
			

		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(0.5,.5,.6);
		glBegin(GL_POLYGON);
		{
			glVertex3f(15,16,0);
			glVertex3f(15,16,45);
			glVertex3f(15,19,45);
			glVertex3f(15,19,0);
			

		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(0.5,.5,.6);
		glBegin(GL_POLYGON);
		{
			glVertex3f(15,19,40);
			glVertex3f(15,19,45);
			glVertex3f(15,-19,45);
			glVertex3f(15,-19,40);
			

		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(1,1,1);
		glBegin(GL_POLYGON);
		{
			glVertex3f(15,-1,35);
			glVertex3f(15,2,35);
			glVertex3f(15,15,27);
			glVertex3f(15,15,25);	
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(1,1,1);
		glBegin(GL_POLYGON);
		{
			glVertex3f(15,1,35);
			glVertex3f(15,-2,35);
			glVertex3f(15,-15,27);
			glVertex3f(15,-15,25);	
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(1,1,1);
		glBegin(GL_POLYGON);
		{
			glVertex3f(15,16,0);
			glVertex3f(15,16,27);
			glVertex3f(15,15,27);
			glVertex3f(15,15,0);	
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(1,1,1);
		glBegin(GL_POLYGON);
		{
			glVertex3f(15,-16,0);
			glVertex3f(15,-16,27);
			glVertex3f(15,-15,27);
			glVertex3f(15,-15,0);	
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(0,1,.61);
		glBegin(GL_POLYGON);
		{
			glVertex3f(15,-1,40);
			glVertex3f(15,16,40);
			glVertex3f(15,16,26);
			glVertex3f(15,-1,36);	
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(0,1,.61);
		glBegin(GL_POLYGON);
		{
			glVertex3f(15,1,40);
			glVertex3f(15,-16,40);
			glVertex3f(15,-16,26);
			glVertex3f(15,1,36);	
		}
		glEnd();
	}glPopMatrix();

	glPushMatrix();
	{
		glColor3f(0.5,0.6,.6);
		glBegin(GL_POLYGON);
		{
			glVertex3f(15,16,40);
			glVertex3f(0,16,40);
			glVertex3f(0,-16,40);
			glVertex3f(15,-16,40);	
		}
		glEnd();
	}glPopMatrix();
}

void Door_Side_Miner()
{
	glPushMatrix();
	{
		glColor3f(0.5,0.5,0.5);
		GLUquadricObj *quadratic;
		quadratic = gluNewQuadric();
		gluCylinder(quadratic,3,3,130,8,8);
	}glPopMatrix();
}

void Wall_01()
{
	glPushMatrix();
	{
		glBegin(GL_POLYGON);
		{
			glColor3f(1,1,1);
			glVertex3f(0,0,0);
			glVertex3f(0,5,0);
			glVertex3f(0,5,105);
			glVertex3f(0,0,105);
		}

		glEnd();
	}
	glPopMatrix();
}


void Wall_02()
{
	glPushMatrix();
	{
		glBegin(GL_POLYGON);
		{
			glColor3f(1,1,1);
			glVertex3f(0,0,4);
			glVertex3f(0,41,4);
			glVertex3f(0,41,10);
			glVertex3f(0,0,10);
		}

		glEnd();
	}
	glPopMatrix();
}


void Wall_03()
{
	glPushMatrix();
	{
		glBegin(GL_POLYGON);
		{
			glColor3f(1,1,1);
			glVertex3f(0,0,0);
			glVertex3f(0,12,0);
			glVertex3f(0,12,105);
			glVertex3f(0,0,105);
		}

		glEnd();
	}
	glPopMatrix();
}


//////////////////Main///////////////////

void Red_Building_Roof()
{
	glPushMatrix();
	{
		glTranslatef(0,0,100);
		glColor3f(0,1,1);
		glBegin(GL_QUADS);
		{
			glVertex3f(50,100,0);
			glVertex3f(50,-100,0);
			glVertex3f(-50,-100,0);
			glVertex3f(-50,100,0);

		}glEnd();
	}glPopMatrix();
	
	glPushMatrix();
	{
		glTranslatef(0,0,80);
		glScalef(.4,.4,.4);
		Roof_Dumb();

	}glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(0,50,90);
		glScalef(.2,.2,.2);
		Roof_Dumb();

	}glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0,-50,90);
		glScalef(.2,.2,.2);
		Roof_Dumb();

	}glPopMatrix();

	
	glPushMatrix();
	{
		Red_Building_Door();
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(50,65,60);
		Door_Side_wall();
	}glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(50,-65,60);
		Door_Side_wall();
	}glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(50,40,0);
		Door_Side_Miner();
		
	}glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(50,-40,0);
		Door_Side_Miner();
		
	}glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(50,-40,128);
		glScalef(0.05,0.06,0.04);
		
		 Roof_Dumb();
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(50,40,128);
		glScalef(0.05,0.06,0.04);
		
		 Roof_Dumb();
	}
	glPopMatrix();


	glPushMatrix();
	{
		glTranslatef(35,65,0);
		glScalef(1,1,1.2);
		Door_Side_Door();
	}glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(35,-65,0);
		glScalef(1,1,1.2);
		Door_Side_Door();
	}glPopMatrix();
	
	glPushMatrix();
	{
		glTranslatef(50,41,0);
		Wall_01();
	}glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(50,-46,0);
		Wall_01();
	}glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(50,46,50);
		Wall_02();
	}glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(50,-85,50);
		Wall_02();
	}glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(50,90,0);
		Wall_03();
	}glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(50,-96,0);
		Wall_03();
	}glPopMatrix();

	glPushMatrix();
	{
		
		glTranslatef(50,40,92);
		glScalef(1,1.4,1.9);
		Wall_02();
	}glPopMatrix();

	glPushMatrix();
	{
		
		glTranslatef(50,-97,92);
		glScalef(1,1.4,1.9);
		Wall_02();
	}glPopMatrix();

	glPushMatrix();
	{
		
		glTranslatef(50,-45,99);
		glScalef(1,2,1.5);
		Wall_02();
	}glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(50,84,0);
		Door_Side_Miner();

	}glPopMatrix();

}


void Red_Building_Part_01()
{
	glColor3f(1,1,1);
	glPushMatrix();
	{
		glBegin(GL_QUADS);
		{
			glVertex3f(0,0,40);
			glVertex3f(0,20,40);
			glVertex3f(0,20,0);
			glVertex3f(0,0,0);

		}glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	{
		glLineWidth(1.0);
		glColor3f( 0,0,0);
		glBegin(GL_LINES);
		{

			glVertex3f(0,5,35);
			glVertex3f(0,15,35);
		}

		glEnd();
		
	}glPopMatrix();
	glColor3f(0.7,0.7,0.7);
	glScalef(2.5,.01, 12);
	glutSolidCube(1);
}


void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(BLACK, 0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera is looking?
	//3. Which direction is the camera's UP direction?

	//instead of CONSTANT information, we will define a circular path.
	//gluLookAt(0,-200,100,	0,0,0,	0,0,1);

	gluLookAt(position[0],position[1],position[2],
			lookat[0], lookat[1], lookat[2],
			up[0], up[1], up[2]);
	//NOTE: the camera still CONSTANTLY looks at the center
	// cameraAngle is in RADIAN, since you are using inside COS and SIN
	
	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	//add objects
	//rotate this rectangle around the Z axis

	//some gridlines along the field
	int i;

	// draw the two AXES
	glColor3f(1, 1, 1);	//100% white
	glBegin(GL_LINES);{
		//Y axis
		glVertex3f(0, -150, 0);	// intentionally extended to -150 to 150, no big deal
		glVertex3f(0,  150, 0);

		//X axis
		glVertex3f(-150, 0, 0);
		glVertex3f( 150, 0, 0);
	}glEnd();



	//draw first two : as half spheres: bottom half.
	/// the cutting plane equation: z = 30
	/// we will keep the points with
	//		z <= 30
	//OR	0.x + 0.y + 1.z - 30 <= 0
	//OR	0.x + 0.y - 1.z + 30 >= 0	//// standard format:: ax + by + cz + d >= 0
	
	

	//drawing begins
	glColor3f(.86,0.86,0.86);
	glTranslatef(0,0,5);

	glPushMatrix();{
		glColor3f(.95,.95,.95);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,gombujChoto);
			glNormal3f(0.0,0.0,1.0);
			glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(-50,-50,5.01);
				glTexCoord2f(10,0); glVertex3f(-50,50,5.01);
				glTexCoord2f(10,10); glVertex3f(50,50,5.01);
				glTexCoord2f(0,10); glVertex3f(50,-50,5.01);
			glEnd();
			glPushMatrix();{
				glColor3f(1,1,1);
			glBegin(GL_POLYGON);
				glTexCoord2f(0,0); glVertex3f(-60,-60,-5);
				glTexCoord2f(10,0); glVertex3f(-60,60,-5);
				glTexCoord2f(10,10); glVertex3f(60,60,-5);
				glTexCoord2f(0,10); glVertex3f(60,-60,-5);
			glEnd();
			}glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		glScalef(10, 10, 1);
		glutSolidCube(10);
	}glPopMatrix();

	glPushMatrix();{
		glPushMatrix();{
			glColor3f(.95,.95,.95);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,redBrick);
				glNormal3f(0.0,0.0,1.0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(-150,-60,-5.01);
					glTexCoord2f(12,0); glVertex3f(-150,60,-5.01);
					glTexCoord2f(12,30); glVertex3f(150,60,-5.01);
					glTexCoord2f(0,30); glVertex3f(150,-60,-5.01);
				glEnd();
				glDisable(GL_TEXTURE_2D);

				glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,redBrick);
				glNormal3f(0.0,1.0,0.0);
				glBegin(GL_POLYGON);
				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(-150,-60,-25);
					glTexCoord2f(2,0); glVertex3f(-150,-60,-5.01);
					glTexCoord2f(2,30); glVertex3f(150,-60,-5.01);
					glTexCoord2f(0,30); glVertex3f(150,-60,-25);
				glEnd();
			glDisable(GL_TEXTURE_2D);
		}glPopMatrix();

		glColor3f(0.6549,.27343,.2196);
		glTranslatef(0,0,-15);
		glScalef(30, 12, 1.7);
		glutSolidCube(10);
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(130, 0, -8);
		glRotatef(180, 0,0,1);
		glScalef(.3,.3,.3);
		Red_Building_Roof();
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(130, 0, -7.2);
		//glRotatef(180, 0,0,1);
		glScalef(.3,.3,.3);
		Red_Building_Roof();
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(130, -30, 8);
		//glRotatef(180, 0,0,1);
		glScalef(1,.01,1);
		glutSolidCube(30);
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(130, 30, 8);
		//glRotatef(180, 0,0,1);
		glScalef(1,.01,1);
		glutSolidCube(30);
	}glPopMatrix();

	glPushMatrix();{
		glColor3f(1,0,0);
		glTranslatef(0,0,-15);
		glScalef(30, 12, 1.5);
		glutSolidCube(10);
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(-130, 0, -8);
		glRotatef(180, 0,0,1);
		glScalef(.3,.3,.3);
		Red_Building_Roof();
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(-130, 0, -7.2);
		//glRotatef(180, 0,0,1);
		glScalef(.3,.3,.3);
		Red_Building_Roof();
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(-130, -30, 8);
		//glRotatef(180, 0,0,1);
		glScalef(1,.01,1);
		glutSolidCube(30);
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(-130, 30, 8);
		//glRotatef(180, 0,0,1);
		glScalef(1,.01,1);
		glutSolidCube(30);
	}glPopMatrix();

	
	for(i = 0; i < 37; i++){
		glPushMatrix();{
			
			glColor3f(1,1,1);
			
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,wallTex);
				glNormal3f(0.0,1.0,0.0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(-46 + (i*2.5),50.01,-5);
					glTexCoord2f(0,1); glVertex3f(-46 + (i*2.5),50.01,6);
					glTexCoord2f(1,1); glVertex3f(-43.5 + (i*2.5),50.01,6);
					glTexCoord2f(1,0); glVertex3f(-43.5 + (i*2.5),50.01,-5);
				glEnd();
			glDisable(GL_TEXTURE_2D);
			
		}glPopMatrix();
	}

	for(i = 0; i < 37; i++){
		glPushMatrix();{
			//glTranslatef(50.1, -45 + (i*2.5), 1);
			glRotatef(270,0,0,1);
			glPushMatrix();{	
			glColor3f(1,1,1);
			
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,wallTex);
				glNormal3f(0.0,1.0,0.0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(-46 + (i*2.5),50.01,-5);
					glTexCoord2f(0,1); glVertex3f(-46 + (i*2.5),50.01,6);
					glTexCoord2f(1,1); glVertex3f(-43.5 + (i*2.5),50.01,6);
					glTexCoord2f(1,0); glVertex3f(-43.5 + (i*2.5),50.01,-5);
				glEnd();
			glDisable(GL_TEXTURE_2D);
		}glPopMatrix();
		}glPopMatrix();
	}

	for(i = 0; i < 37; i++){
		glPushMatrix();{
			//glTranslatef(-45 + (i*2.5), -50.01, 1);
			glRotatef(180,0,0,1);
			glPushMatrix();{	
			glColor3f(1,1,1);
			
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,wallTex);
				glNormal3f(0.0,1.0,0.0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(-46 + (i*2.5),50.01,-5);
					glTexCoord2f(0,1); glVertex3f(-46 + (i*2.5),50.01,6);
					glTexCoord2f(1,1); glVertex3f(-43.5 + (i*2.5),50.01,6);
					glTexCoord2f(1,0); glVertex3f(-43.5 + (i*2.5),50.01,-5);
				glEnd();
			glDisable(GL_TEXTURE_2D);
		}glPopMatrix();
		}glPopMatrix();
	}
	
	for(i = 0; i < 37; i++){
		glPushMatrix();{
			//glTranslatef(-50.1, -45 + (i*2.5), 1);
			glRotatef(90,0,0,1);
			glPushMatrix();{	
			glColor3f(1,1,1);
			
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,wallTex);
				glNormal3f(0.0,1.0,0.0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(-47.5 + (i*2.5),50.01,-5);
					glTexCoord2f(0,1); glVertex3f(-47.5 + (i*2.5),50.01,6);
					glTexCoord2f(1,1); glVertex3f(-45 + (i*2.5),50.01,6);
					glTexCoord2f(1,0); glVertex3f(-45 + (i*2.5),50.01,-5);
				glEnd();
			glDisable(GL_TEXTURE_2D);
		}glPopMatrix();
		}glPopMatrix();
	}

	//pillar drawing st
	glPushMatrix();{
			glTranslatef(-50,50,-5);
			glColor3f(.7,.7,.7);
			pillar();
	}glPopMatrix();	

	glPushMatrix();{
			glTranslatef(50,-50,-5);
			glColor3f(.7,.7,.7);
			pillar();
	}glPopMatrix();	

	glPushMatrix();{
			glTranslatef(-50,-50,-5);
			glColor3f(.7,.7,.7);
			pillar();
	}glPopMatrix();	

	glPushMatrix();{
			glTranslatef(50,50,-5);
			glColor3f(.7,.7,.7);
			pillar();
	}glPopMatrix();	
	//pillar draw ends

	//entrance object start
	glPushMatrix();{
		glTranslatef(0,-25,18);
		front();	
	}glPopMatrix();

	glPushMatrix();{
		glRotatef(90, 0,0,1);
		glTranslatef(0,-25,18);
		front();	
	}glPopMatrix();

	glPushMatrix();{
		glRotatef(180, 0,0,1);
		glTranslatef(0,-25,18);
		front();	
	}glPopMatrix();

	glPushMatrix();{
		glRotatef(270, 0,0,1);
		glTranslatef(0,-25,18);
		front();	
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(24,-24,16.5);
		glRotatef(45, 0,0,1);
		glScalef(.6,.2,.9);
		entranceSmaller();	
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(-24,-24,16.5);
		glRotatef(-45, 0,0,1);
		glScalef(.6,.2,.9);
		entranceSmaller();	
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(-24,24,16.5);
		glRotatef(225, 0,0,1);
		glScalef(.6,.2,.9);
		entranceSmaller();	
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(24,24,16.5);
		glRotatef(135, 0,0,1);
		glScalef(.6,.2,.9);
		entranceSmaller();	
	}glPopMatrix();
	
	//chad begin
	glPushMatrix();{
		glTranslatef(0,0, 29);
		glColor3f(.85,.85,.85);
		//glScalef(5,5,.01);
		glPushMatrix();{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,gombujChoto);
				glNormal3f(0.0,0.0,1.0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(-23,-23,.01);
					glTexCoord2f(5,0); glVertex3f(-23,23,.01);
					glTexCoord2f(5,5); glVertex3f(23,23,.01);
					glTexCoord2f(0,5); glVertex3f(23,-23,.01);
				glEnd();
			glDisable(GL_TEXTURE_2D);
		}glPopMatrix();
		//glutSolidCube(9);
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(22,0, 29);
		glColor3f(.85,.85,.85);
		//glScalef(1,5,.01);
		glPushMatrix();{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,gombujChoto);
				glNormal3f(0.0,0.0,1.0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(-4.5,-23,.01);
					glTexCoord2f(5,0); glVertex3f(-4.5,23,.01);
					glTexCoord2f(5,1); glVertex3f(4.5,23,.01);
					glTexCoord2f(0,1); glVertex3f(4.5,-23,.01);
				glEnd();
			glDisable(GL_TEXTURE_2D);
		}glPopMatrix();
		//glutSolidCube(9);
	}glPopMatrix();

	glPushMatrix();{
		glRotatef(90,0,0,1);
		glTranslatef(22,0, 29);
		glColor3f(.85,.85,.85);
		//glScalef(1,5,.01);
		glPushMatrix();{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,gombujChoto);
				glNormal3f(0.0,0.0,1.0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(-4.5,-23,.01);
					glTexCoord2f(5,0); glVertex3f(-4.5,23,.01);
					glTexCoord2f(5,1); glVertex3f(4.5,23,.01);
					glTexCoord2f(0,1); glVertex3f(4.5,-23,.01);
				glEnd();
			glDisable(GL_TEXTURE_2D);
		}glPopMatrix();
		//glutSolidCube(9);
	}glPopMatrix();

	glPushMatrix();{
		glRotatef(180,0,0,1);
		glTranslatef(22,0, 29);
		glColor3f(.85,.85,.85);
		glPushMatrix();{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,gombujChoto);
				glNormal3f(0.0,0.0,1.0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(-4.5,-23,.01);
					glTexCoord2f(5,0); glVertex3f(-4.5,23,.01);
					glTexCoord2f(5,1); glVertex3f(4.5,23,.01);
					glTexCoord2f(0,1); glVertex3f(4.5,-23,.01);
				glEnd();
			glDisable(GL_TEXTURE_2D);
		}glPopMatrix();
		//glScalef(1,5,.01);
		//glutSolidCube(9);
	}glPopMatrix();

	glPushMatrix();{
		glRotatef(-90,0,0,1);
		glTranslatef(22,0, 29);
		glColor3f(.85,.85,.85);
		glPushMatrix();{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,gombujChoto);
				glNormal3f(0.0,0.0,1.0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0,0); glVertex3f(-4.5,-23,.01);
					glTexCoord2f(5,0); glVertex3f(-4.5,23,.01);
					glTexCoord2f(5,1); glVertex3f(4.5,23,.01);
					glTexCoord2f(0,1); glVertex3f(4.5,-23,.01);
				glEnd();
			glDisable(GL_TEXTURE_2D);
		}glPopMatrix();
		//glScalef(1,5,.01);
		//glutSolidCube(9);
	}glPopMatrix();
	//chad ends

	//chad gombuj begins
	glPushMatrix();{
		glTranslatef(15,15, -70);
		glColor3f(.3,.3,.3);
		glScalef(3.2,3.2,2.5);
		gombuj();
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(15,-15, -70);
		glColor3f(.3,.3,.3);
		glScalef(3.2,3.2,2.5);
		gombuj();
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(-15,15, -70);
		glColor3f(.3,.3,.3);
		glScalef(3.2,3.2,2.5);
		gombuj();
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(-15,-15, -70);
		glColor3f(.3,.3,.3);
		glScalef(3.2,3.2,2.5);
		gombuj();
	}glPopMatrix();
	//chad gombuj ends

	
	//big gombuj start
	glPushMatrix();{
		glTranslatef(0,0,30);
		glColor3f(.98,.98,.98);
		glPushMatrix();{
		//glTranslatef(0,60,0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, domeBase3); // here texid corresponds a bitmap image.
			gluQuadricNormals(IDquadric,GLU_SMOOTH);
			gluQuadricTexture(IDquadric, GLU_TRUE);
			gluCylinder(IDquadric, 12, 12, 6, 30, 30);
			glDisable(GL_TEXTURE_2D);
		}glPopMatrix();
		//gluCylinder(quadratic, 12, 12, 6, 30, 30);
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(0,0,36);
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, domeBase2); // here texid corresponds a bitmap image.
			gluQuadricNormals(IDquadric,GLU_SMOOTH);
			gluQuadricTexture(IDquadric, GLU_TRUE);
			gluCylinder(IDquadric, 12, 12, 8, 30, 30);
		glDisable(GL_TEXTURE_2D);
		
		//gluCylinder(quadratic, 12, 12, 8, 30, 30);
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(0,0,44);
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, domeBase4); // here texid corresponds a bitmap image.
			gluQuadricNormals(IDquadric,GLU_SMOOTH);
			gluQuadricTexture(IDquadric, GLU_TRUE);
			gluCylinder(IDquadric, 12, 14, 4, 30, 30);
		glDisable(GL_TEXTURE_2D);
		
	}glPopMatrix();

	glPushMatrix();{
		glTranslatef(0,0, -247);
		glScalef(10.7,10.7,5);
		glPushMatrix();{
			glColor3f(1,1,1);
			glTranslatef(0, 0, 60);
			double equ[4];

			equ[0] = 0;	//0.x
			equ[1] = 0;	//0.y
			equ[2] = 1;//-1.z
			equ[3] = 1;//30

			glClipPlane(GL_CLIP_PLANE0,equ);

			//now we enable the clip plane
			glScalef(.35,.35,.7);
			glEnable(GL_CLIP_PLANE0);{
				//glutSolidSphere(4,20,20);
				glPushMatrix();{
					//glTranslatef(0,-60,0);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, gombujBoro);
					gluQuadricNormals(IDquadric,GLU_SMOOTH);
					gluQuadricTexture(IDquadric, GLU_TRUE);
					gluSphere(IDquadric,4,20,20);
				}glPopMatrix();
			}glDisable(GL_CLIP_PLANE0);
			}glPopMatrix();

			glPushMatrix();{
				glColor3f(.3,.3,.3);
				glTranslatef(0,0,62.64);
				glutWireCone(.5,.8,30,4);
			}glPopMatrix();

			glPushMatrix();{
				glColor3f(.90625,.734375, .3828125);
				glTranslatef(0,0,63.4);
				glutSolidCone(.05,.7,10,10);
			}glPopMatrix();

			glPushMatrix();{
				glColor3f(.90625,.734375, .3828125);
				glTranslatef(0,0,64.2);
				glScalef(1,1,3);
				glutSolidSphere(.1,10,10);
			}glPopMatrix();

			glPushMatrix();{
				glColor3f(.90625,.734375, .3828125);
				glTranslatef(0,0,64.5);
				glutSolidCone(.02,.7,10,10);
			}glPopMatrix();

			glPushMatrix();{
				glColor3f(.90625,.734375, .3828125);
				glTranslatef(0,0,64.9);
				glScalef(1,1,4);
				glutSolidSphere(.05,10,10);
			}glPopMatrix();

			glPushMatrix();{
				glColor3f(.90625,.734375, .3828125);
				glTranslatef(0,0,65);
				glutSolidCone(.02,.8,10,10);
			}glPopMatrix();
	}glPopMatrix();
	//big gombuj ends


	//entrance object end

	//drawing ends

	
	//////// ------ NOTE ---- ORDER matters. compare last two spheres!

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}



void animate(){
	//codes for any changes in Camera
	cameraAngle += cameraAngleRotate;
	// camera will rotate at 0.002 radians per frame.	// keep the camera steady NOW!!
	//defuse
	//codes for any changes in Models
	position_light[0] = -100.0*cos(cameraAngle);
	position_light[1] = -100.0*sin(cameraAngle);
	glLightfv(GL_LIGHT1, GL_POSITION, position_light);  
	//rectAngle -= 1;

	//MISSING SOMETHING? -- YES: add the following
	glutPostRedisplay();	//this will call the display AGAIN
}

void loadMatrix(){
	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	
	double val = sqrt(m[2]*m[2] + m[6]*m[6] + m[10]*m[10]);
	
	fdVector[0] = m[2]/val;
	fdVector[1] = m[6]/val;
	fdVector[2] = m[10]/val;
	
	val = sqrt(m[1]*m[1] + m[5]*m[5] + m[9]*m[9]);
	
	upVector[0] = m[1]/val;
	upVector[1] = m[5]/val;
	upVector[2] = m[9]/val;

	val = sqrt(m[0]*m[0] + m[4]*m[4] + m[8]*m[8]);
	
	alongVector[0] = m[0]/val;
	alongVector[1] = m[4]/val;
	alongVector[2] = m[8]/val;
}

void keyboardListener(unsigned char key, int x,int y){
	int i;
	switch(key){

		case '1':	//pitch +
			
			glRotatef(-angle, alongVector[0], alongVector[1], alongVector[2]);
			loadMatrix();
			
			for(i = 0; i < 3; i++){
				up[i] = upVector[i];
				lookat[i] = position[i]-fdVector[i];
			}
			break;

		case '2':	//pitch -
		
			glRotatef(angle, alongVector[0], alongVector[1], alongVector[2]);
			loadMatrix();
			
			for(i = 0; i < 3; i++){
				up[i] = upVector[i];
				lookat[i] = position[i]-fdVector[i];
			}
			break;

		case '3':	//yaw +
			
			glRotatef(-angle, upVector[0], upVector[1], upVector[2]);
			loadMatrix();
			
			for(i = 0; i < 3; i++){
				lookat[i] = position[i]-fdVector[i];
			}
			break;

		case '4':	//yaw -
			
			glRotatef(angle, upVector[0], upVector[1], upVector[2]);
			loadMatrix();
			
			for(i = 0; i < 3; i++){
				lookat[i] = position[i]-fdVector[i];
			}
			break;

		case '5':	//roll left
			
			glRotatef(angle, fdVector[0], fdVector[1], fdVector[2]);
			loadMatrix();
			
			for(i = 0; i < 3; i++){
				up[i] = upVector[i];
			}
			break;
		case '6': //roll right
			glRotatef(-angle, fdVector[0], fdVector[1], fdVector[2]);
			loadMatrix();

			for(i = 0; i < 3; i++){
				up[i] = upVector[i];
			}
			break;
		case '7': //point light white
			glDisable(GL_LIGHT0);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, diffusePoint_white);
			glLightfv(GL_LIGHT1, GL_POSITION, position_light);
			glEnable(GL_LIGHT1);
			break;
		case '-': //move back to ambinet light

			glDisable(GL_LIGHT1);
			
			break;
		case '/': //no light

			glDisable(GL_LIGHT0);
			
			break;
		case '8': //point light blue
			glDisable(GL_LIGHT0);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, diffusePoint_Blue);
			glLightfv(GL_LIGHT1, GL_POSITION, position_light);
			glEnable(GL_LIGHT1);
			
			break;
		case '9': // rotate light source
			cameraAngleRotate = .01;
			glLightfv(GL_LIGHT1, GL_POSITION, position_light);
			break;
		case '0': // stop rotating light source
			cameraAngleRotate = 0;
		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	int i;
	
	
	double val = sqrt(m[2]*m[2] + m[6]*m[6] + m[10]*m[10]);
	
	fdVector[0] = m[2]/val;
	fdVector[1] = m[6]/val;
	fdVector[2] = m[10]/val;
	
	val = sqrt(m[1]*m[1] + m[5]*m[5] + m[9]*m[9]);
	
	upVector[0] = m[1]/val;
	upVector[1] = m[5]/val;
	upVector[2] = m[9]/val;

	val = sqrt(m[0]*m[0] + m[4]*m[4] + m[8]*m[8]);
	
	alongVector[0] = m[0]/val;
	alongVector[1] = m[4]/val;
	alongVector[2] = m[8]/val;

	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			for(i = 0; i < 3; i++){
				position[i] += fdVector[i] * offset;
				lookat[i] += fdVector[i] * offset;
			}
			break;
		case GLUT_KEY_UP:	
			for(i = 0; i < 3; i++){
				position[i] -= fdVector[i] * offset;
				lookat[i] -= fdVector[i] * offset;
			}
			break;

		case GLUT_KEY_RIGHT:
			for(i = 0; i < 3; i++){
				position[i] += alongVector[i] * offset;
				lookat[i] += alongVector[i] * offset;
			}
			break;
		case GLUT_KEY_LEFT:
			for(i = 0; i < 3; i++){
				position[i] -= alongVector[i] * offset;
				lookat[i] -= alongVector[i] * offset;
			}
			break;

		case GLUT_KEY_PAGE_UP:
			for(i = 0; i < 3; i++){
				position[i] += upVector[i] * offset;
				lookat[i] += upVector[i] * offset;
			}
			break;
		case GLUT_KEY_PAGE_DOWN:
			for(i = 0; i < 3; i++){
				position[i] -= upVector[i] * offset;
				lookat[i] -= upVector[i] * offset;
			}
			break;

		case GLUT_KEY_INSERT:
			glDisable(GL_LIGHT1);
			glEnable(GL_LIGHT0);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, light_spot);
			//glLightf(GL_LIGHT0,GL_SPOT_CUTOFF, 30.0);
			//glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,lightDir_spot1);
			glLightfv(GL_LIGHT0,GL_POSITION,lightDir_spot);



     	break;

		case GLUT_KEY_HOME:
			
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient_night);
			glEnable(GL_COLOR_MATERIAL);
			glEnable(GL_LIGHTING);
			break;
		case GLUT_KEY_END:
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient_day);
			glEnable(GL_COLOR_MATERIAL);
			glEnable(GL_LIGHTING);
			break;

		default:
			break;
	}
}



void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				cameraAngleDelta = -cameraAngleDelta;	
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}


void init(){
	//codes for initialization
	cameraAngle = 0;
	cameraAngleRotate = 0;	//// init the cameraAngle
	cameraAngleDelta = 0.0002;
	rectAngle = 0;
	canDrawGrid = true;
	cameraHeight = 150;
	cameraRadius = 150;
	loadAssets();
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	//clear the screen
	glClearColor(BLACK, 0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);
	
	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(70,	1,	0.1,	10000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
	//lighting
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	//global_ambient_day[] = { 15.3, 15.3, 15.3, 1.0 }; //white light
	//global_ambient_night[] = { 1.3, 1.3, 1.3, 1.0 }; //white light
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient_night);
	glEnable(GL_LIGHTING);
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Tajmahal Renderer");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	//ADD keyboard listeners:
	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

	//ADD mouse listeners:
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
