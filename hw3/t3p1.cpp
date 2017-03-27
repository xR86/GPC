#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include <string.h>

#include <GL/freeglut.h>


#define PI 3.141592
// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;
int nivel = 0;


//Display bitmap
void displayBitmap(const char* str){
  for(int i = 0; i < strlen(str); i++){
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
  }
  
}


//other classes

class C2coord
{
public:
  C2coord() 
  {
    m.x = m.y = 0;
  }

  C2coord(double x, double y) 
  {
    m.x = x;
    m.y = y;
  }

  C2coord(const C2coord &p) 
  {
    m.x = p.m.x;
    m.y = p.m.y;
  }

  C2coord &operator=(C2coord &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  int operator==(C2coord &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

protected:
  struct SDate
  {
    double x,y;
  } m;
};

class CPunct : public C2coord
{
public:
  CPunct() : C2coord(0.0, 0.0)
  {}

  CPunct(double x, double y) : C2coord(x, y)
  {}

  CPunct &operator=(const CPunct &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  void getxy(double &x, double &y)
  {
    x = m.x;
    y = m.y;
  }

  void setxy(double x, double y) //new, should work
  {
    m.x = x;
    m.y = y;
  }

  int operator==(CPunct &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

  void marcheaza()
  {
    glBegin(GL_POINTS);
      glVertex2d(m.x, m.y);
    glEnd();
  }

  void print(FILE *fis)
  {
    fprintf(fis, "(%+f,%+f)", m.x, m.y);
  }

};

class CVector : public C2coord
{
public:
  CVector() : C2coord(0.0, 0.0)
  {
    normalizare();
  }

  CVector(double x, double y) : C2coord(x, y)
  {
    normalizare();
  }

  CVector &operator=(CVector &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  int operator==(CVector &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

  CPunct getDest(CPunct &orig, double lungime)
  {
    double x, y;
    orig.getxy(x, y);
    CPunct p(x + m.x * lungime, y + m.y * lungime);
    return p;
  }

  void rotatie(double grade)
  {
    double x = m.x;
    double y = m.y;
    double t = 2 * (4.0 * atan(1)) * grade / 360.0;
    m.x = x * cos(t) - y * sin(t);
    m.y = x * sin(t) + y * cos(t);
    normalizare();
  }

  void deseneaza(CPunct p, double lungime) 
  {
    double x, y;
    p.getxy(x, y);
    //glColor3f(1.0, 0.1, 0.1);
    //glColor3f(0.1, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
      glVertex2d(x, y);
      glVertex2d(x + m.x * lungime, y + m.y * lungime);
    glEnd();
  }

  void print(FILE *fis)
  {
    fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
  }

private:
  void normalizare()
  {
    double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
    if (d != 0.0) 
    {
      C2coord::m.x = C2coord::m.x * 1.0 / d;
      C2coord::m.y = C2coord::m.y * 1.0 / d;
    }
  }
};



class GrilaCarteziana
{
public:
  void grilaCarteziana(int coloane, int randuri)
  {
      //if (x >= -3.0 && x <= 3.0 && y >= -3.0 && y <= 3.0){
      
      double lungimeColoana = 2.0f / coloane;
      double lungimeRand = 2.0f / randuri;

      CVector v(0.0, 1.0);
      CPunct p(-1.0, -1.0); //~<1 will display

      for(int i = 0; i <= coloane; i++){
         v.deseneaza(p, 2);

         double x, y;
         p.getxy(x, y); 
         p.setxy(x + lungimeColoana, y);
      }

      v.rotatie(-90);
      p.setxy(-1.0, 1.0);
      for(int j = 0; j <= randuri; j++){
         v.deseneaza(p, 2);

         double x, y;
         p.getxy(x, y); 
         p.setxy(x, y - lungimeRand);
      }
  }

  void drawFilledCircle(float x, float y, float radius){
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

  void writePixel(int line, int column, int coloane, int randuri){
      glColor3f(0.5, 0.5, 0.5);
      double lungimeColoana = 2.0f / coloane;
      double lungimeRand = 2.0f / randuri;

      drawFilledCircle(-1.0 + column * lungimeColoana, 1.0 - line * lungimeRand, lungimeColoana/2.5);
  }

  void drawVector(CPunct p, CVector v, float lungime, float rotatie){
     
      glLineWidth(3);
      glColor3f(1.0, 0.1, 0.1);

      v.rotatie(rotatie);
      v.deseneaza(p, lungime);

      glLineWidth(1);
  }

  void drawLine(float x0, float y0, float xn, float yn){
      float sensitivity = 10.0;
      float pasX = xn / sensitivity;
      float pasY = yn / sensitivity;

      float xM = x0 - 1; //normalize to actual origin
      float yM = y0 - 1;

      float overflowLength = 1.5;
      if(xn > 1)
         xn = overflowLength;
      if(yn > 1)
         yn = overflowLength;

      glLineWidth(3);
      glColor3f(1.0, 0.1, 0.1);
      glBegin(GL_LINE_STRIP); 
         while(xM < xn && yM < yn){
            glVertex2f(xM,yM);

            xM += pasX;
            yM += pasY;
         }
      glEnd();
      glLineWidth(1);
  }

  // procedure AfisareSegmentDreapta3 (x0,y0,xn,yn : Z,
  //      var M : list of (Z x Z))
  // {
  //   // valoarea initiala a variabile de decizie
  //   // dx, dy sunt constante - a se vedea mai sus
  //   int d = 2 * dy - dx;
  //   int dE = 2 * dy;
  //   int dNE = 2 * (dy - dx);
  //   int x = x0, y = y0;
  //   M = (x,y);
  //   while (x < xn)
  //   {
  //     if (d <= 0) {
  //       /* alegem E */ d += dE; x++; 
  //     }
  //     else { 
  //       /* alegem NE */ d += dNE; x++; y++; 
  //     }
  //     M = M . (x, y);
  //   }
  // }
  
  void AfisareSegmentDreapta3(){
      //drawLine + algo + adapted for other cases (besides N-NE)
      //
  }

  void afisare(int coloane, int randuri)
  {

   glColor3f(0.1, 0.1, 0.1);
   grilaCarteziana(coloane, randuri);

   drawLine(0, 0, 5, 5);
  }
};




// ---------------DISPLAYS------------------------------------

//Grila carteziana
void Display5() {

  //char display
  glRasterPos2d(-1.0,-0.9);
  displayBitmap("AfisareSegmentDreapta3");

  glPushMatrix();
  glLoadIdentity();
  glScaled(0.8, 0.8, 1);

  GrilaCarteziana csc;



  int coloane = 12;
  int randuri = 12;

  csc.afisare(coloane, randuri);

  CVector v(0.0, 1.0);
  CPunct p(-1.0, -1.0);

  csc.drawVector(p, v, 2, -30);
  // approximation
  csc.writePixel(12, 0, coloane, randuri);
  csc.writePixel(11, 1, coloane, randuri);
  csc.writePixel(10, 1, coloane, randuri);
  csc.writePixel(9, 2, coloane, randuri);
  csc.writePixel(8, 3, coloane, randuri);
  csc.writePixel(7, 3, coloane, randuri);
  csc.writePixel(6, 4, coloane, randuri);


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
    case '0':
      glClear(GL_COLOR_BUFFER_BIT);
      nivel = 0;
      fprintf(stderr, "nivel = %d\n", nivel);
      break;
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
