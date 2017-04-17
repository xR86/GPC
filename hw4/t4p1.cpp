#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <vector>
#include <iostream>
#include <string.h>

#include <GL/freeglut.h>


#define dim 300

#define PI 3.141592
using namespace std;

unsigned char prevKey;
int nivel = 0;

//Display bitmap
void displayBitmap(const char* str){
  for(int i = 0; i < strlen(str); i++){
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
  }

}


class GrilaCarteziana
{
public:
  GrilaCarteziana(int linii, int coloane)
  {
    this->linii = linii ;
    this->coloane = coloane;
    this->lungimeLinie = 2.0/linii;
    this->lungimeColoana = 2.0/coloane;
  }
  void draw()
  {
    glColor3f(0.1, 0.9, 0.1);

    for(int i = 0; i <= linii; i++)
    {
        glBegin(GL_LINES);
        glVertex2d(-1, -1+lungimeLinie*i);
        glVertex2d(1, -1+lungimeColoana*i);
        glEnd();
    }

    for(int j = 0; j <= coloane; j++)
    {
        glBegin(GL_LINES);
        glVertex2d(-1+lungimeLinie*j, -1);
        glVertex2d(-1+lungimeColoana*j, 1);
        glEnd();
    }
  }
  void drawFilledCircle(float x, float y, float radius)
  {
      int i;
      int triangleAmount = 20; //# of triangles used to draw circle

      //float radius = 0.8f; //radius
      float twicePi = 2.0f * PI;

      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glBegin(GL_TRIANGLE_FAN);
         glVertex2f(x, y); // center of circle
         for(i = 0; i <= triangleAmount;i++) {
           glVertex2f(
               x + (radius * cos(i *  twicePi / triangleAmount)),
               y + (radius * sin(i * twicePi / triangleAmount))
           );
         }
      glEnd();
   }

  void writePixel(pair<int, int> x)
  {
      glColor3f(0.5, 0.5, 0.5);

      drawFilledCircle(-1.0 + x.first * lungimeColoana, -1.0 + x.second * lungimeLinie, lungimeColoana/2.5);

       glEnd();
  }

  void drawLine(int x0, int y0, int xn, int yn)
  {
    glColor3f(1, 0.3, 0.1);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2d(-1+lungimeLinie*x0, -1+lungimeColoana*y0);
    glVertex2d(-1+lungimeLinie*xn, -1+lungimeColoana*yn);
    glEnd();
  }

  //exe1
  void AfisareCerc4();
  //exe2
  void UmplereElipsa();
  //exe3
  void UmplerePoligon();

private:
  int linii, coloane;
  float lungimeLinie, lungimeColoana;
};




// ---------------DISPLAYS------------------------------------

//Grila carteziana
void Display5() {

  //char display
  //glRasterPos2d(-1.0,-0.9);
  displayBitmap("AfisareSegmentDreapta3");

  glScaled(0.8, 0.8, 1);
  int first_x0 = 0, first_y0 = 0, first_xn = 15, first_yn = 7;
  int second_x0 = 0, second_y0 = 15, second_xn = 15, second_yn = 10;

  GrilaCarteziana csc(15, 15);
  glPushMatrix();
  glLoadIdentity();
  csc.draw();
  //csc.writePixel(make_pair(14, 1));
  csc.drawLine(first_x0, first_y0, first_xn, first_yn);
  csc.drawLine(second_x0, second_y0, second_xn, second_yn);
  
  glPopMatrix();
}


void Init(void) {

   glClearColor(1.0,1.0,1.0,1.0);
   glLineWidth(1);
   glPointSize(3);

   glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void)
{
  switch(prevKey)
  {
    case '5':
      glClear(GL_COLOR_BUFFER_BIT);
      Display5();
      break;
    default:
      break;
  }

  glFlush();
}

void Reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void KeyboardFunc(unsigned char key, int x, int y)
{
   prevKey = key;
   if (key == 27) // escape
      exit(0);
   glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y)
{
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);

  glutInitWindowSize(dim, dim);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

  glutCreateWindow (argv[0]);

  Init();

  glutReshapeFunc(Reshape);
  glutKeyboardFunc(KeyboardFunc);
  glutMouseFunc(MouseFunc);
  glutDisplayFunc(Display);

  glutMainLoop();

  return 0;
}
