#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include <string.h>

#include <GL/freeglut.h>

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
    glColor3f(1.0, 0.1, 0.1);
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

class CCurbaKoch
{
public:
  void segmentKoch(double lungime, int nivel, CPunct &p, CVector v)
  {
    CPunct p1;
    if (nivel == 0) 
    {
      v.deseneaza(p, lungime);
    }
    else
    {
  //    v.print(stderr);
  //    fprintf(stderr, "\n");
      segmentKoch(lungime / 3.0, nivel - 1, p, v);
      p1 = v.getDest(p, lungime / 3.0);
      v.rotatie(60);
  //    v.print(stderr);
  //    fprintf(stderr, "\n");
      segmentKoch(lungime / 3.0, nivel - 1, p1, v);
      p1 = v.getDest(p1, lungime / 3.0);
      v.rotatie(-120);
  //    v.print(stderr);
  //    fprintf(stderr, "\n");
      segmentKoch(lungime / 3.0, nivel - 1, p1, v);
      p1 = v.getDest(p1, lungime / 3.0);
      v.rotatie(60);
  //    v.print(stderr);
  //    fprintf(stderr, "\n");
      segmentKoch(lungime / 3.0, nivel - 1, p1, v);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v1(sqrt(3.0)/2.0, 0.5);
    CPunct p1(-1.0, 0.0);

    CVector v2(0.0, -1.0);
    CPunct p2(0.5, sqrt(3.0)/2.0);

    CVector v3(-sqrt(3.0)/2.0, 0.5);
    CPunct p3(0.5, -sqrt(3.0)/2.0);

    segmentKoch(lungime, nivel, p1, v1);
    segmentKoch(lungime, nivel, p2, v2);
    segmentKoch(lungime, nivel, p3, v3);
  }
};

class CArboreBinar
{
public:
  void arboreBinar(double lungime, int nivel, CPunct &p, CVector v)
  {
    CPunct p1;
    if (nivel == 0) 
    {
      v.deseneaza(p, lungime);
    }
    else
    {
      arboreBinar(lungime, nivel - 1, p, v);
      p1 = v.getDest(p, lungime);

      v.rotatie(-45);
      arboreBinar(lungime / 2.0, nivel - 1, p1, v);

      v.rotatie(90);
      arboreBinar(lungime / 2.0, nivel - 1, p1, v);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v(0.0, -1.0);
    CPunct p(0.0, 1.0);

    arboreBinar(lungime, nivel, p, v);
  }
};

class CArborePerron
{
public:
  void arborePerron(double lungime, 
                    int nivel, 
                    double factordiviziune, 
                    CPunct p, 
                    CVector v)
  {
    assert(factordiviziune != 0);
    CPunct p1, p2;
    if (nivel == 0) 
    {
    }
    else
    {
      v.rotatie(30);
      v.deseneaza(p, lungime);
      p1 = v.getDest(p, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      v.rotatie(-90);
      v.deseneaza(p, lungime);
      p1 = v.getDest(p, lungime);
      p2 = p1;

      v.rotatie(-30);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      p1 = p2;
      v.rotatie(90);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      p2 = p1;

      v.rotatie(30);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      p1 = p2;
      v.rotatie(-90);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v(0.0, 1.0);
    CPunct p(0.0, -1.0);

    v.deseneaza(p, 0.25);
    p = v.getDest(p, 0.25);
    arborePerron(lungime, nivel, 0.4, p, v);
  }
};



class CCurbaHilbert
{
public:
  void curbaHilbert(double lungime, int nivel, CPunct &p, CVector &v, int d)
  {
    if (nivel == 0) 
    {
    }
    else
    {
      v.rotatie(d * 90);
      curbaHilbert(lungime, nivel - 1, p, v, -d);

      v.deseneaza(p, lungime);
      p = v.getDest(p, lungime);

      v.rotatie(-d * 90);
      curbaHilbert(lungime, nivel - 1, p, v, d);

      v.deseneaza(p, lungime);
      p = v.getDest(p, lungime);

      curbaHilbert(lungime, nivel - 1, p, v, d);

      v.rotatie(-d * 90);
      v.deseneaza(p, lungime);
      p = v.getDest(p, lungime);
      
      curbaHilbert(lungime, nivel - 1, p, v, -d);

      v.rotatie(d * 90);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v(0.0, 1.0);
    CPunct p(0.0, 0.0);

    curbaHilbert(lungime, nivel, p, v, 1);
  }
};

class CSierpinskiCarpet
{
public:
  void sierpinskiCarpet(double lungime, 
                    int nivel, 
                    double factordiviziune, 
                    CPunct p)
  {
    assert(factordiviziune != 0);
    CPunct tr;
    if (nivel == 0) 
    {
    }
    else
    {
      double x, y;


      p.getxy(x, y);
      CPunct p1(x - 0.5, y + 0.5);

      //if (x >= -1.0 && x <= 1.0 && y >= -1.0 && y <= 1.0){
        //CVector v2 = createSquare(lungime, p1);
        
        tr = createSquare(lungime, p1);
        tr.getxy(x, y);
        //CPunct p2(x - lungime/3, y + lungime/3);
        p1.setxy(x - lungime/3, y + lungime/3);
        
        //sierpinskiCarpet(lungime * factordiviziune, nivel - 1, factordiviziune,  p1);

        //recursed
        lungime = lungime * factordiviziune;

        p1.setxy(x - 0.5, y + 0.5);
        tr = createSquare(lungime, p1);
        tr.getxy(x, y);
        p1.setxy(x + lungime/3, y + lungime/3);

        //recursed
        lungime = lungime * factordiviziune;

        p1.setxy(x - 0.5, y + 0.5);
        tr = createSquare(lungime, p1);
        tr.getxy(x, y);
        p1.setxy(x + lungime/3, y + lungime/3);



        // tr.getxy(x, y);
        // CPunct p2(x, y + lungime / 3.0); // x,y from p1
        // tr = createSquare(lungime, p2);

        //sierpinskiCarpet(lungime * factordiviziune, nivel - 1, factordiviziune,  tr);
      //}
      
    }
  }

  CPunct createSquare(double lungime, CPunct origin){
    CVector v(lungime, 0.0); //orig: 0.0, -1.0 -> schimbat sens
    CPunct p1, p2, p3;
    double x, y;

    v.deseneaza(origin, lungime);
    origin = v.getDest(origin, lungime);

    v.rotatie(-90);
    v.deseneaza(origin, lungime);
    p1 = v.getDest(origin, lungime);

    v.rotatie(-90);
    v.deseneaza(p1, lungime);
    p2 = v.getDest(p1, lungime);

    v.rotatie(-90);
    v.deseneaza(p2, lungime);
    p3 = v.getDest(p2, lungime);

    return p3;
  }

  void afisare(double lungime, int nivel)
  {
    CPunct p(0.0, 0.0); //orig: 0.0, -1.0 -> schimbat origine
    sierpinskiCarpet(lungime, nivel, 0.33, p);
  }
};

class CArborePerronInversat
{
public:
  void arborePerronInversat(double lungime, 
                    int nivel, 
                    double factordiviziune, 
                    CPunct p, 
                    CVector v)
  {
    assert(factordiviziune != 0);
    CPunct p1, p2;
    if (nivel == 0) 
    {
    }
    else
    {
    
      v.rotatie(-45); //orig: 30
      v.deseneaza(p, lungime);
      p1 = v.getDest(p, lungime);
      arborePerronInversat(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      v.rotatie(90); //orig:-90
      v.deseneaza(p, lungime);
      p1 = v.getDest(p, lungime);
      p2 = p1;

      //de aici, mod1
      v.rotatie(15); //orig:-30, mod1: 30
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      arborePerronInversat(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      p1 = p2;
      v.rotatie(-60); //orig: 90, mod1: -90
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      p2 = p1;

      v.rotatie(-90); //orig: 30, mod1: -30
      v.deseneaza(p1, lungime/2); //halfed
      p1 = v.getDest(p1, lungime/2); //halfed
      arborePerronInversat(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      p1 = p2;
      v.rotatie(90 + 15); //orig: -90, mod1: 90
      v.deseneaza(p1, lungime/2); //halfed
      p1 = v.getDest(p1, lungime/2); //halfed
      arborePerronInversat(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v(0.0, -1.0); //orig: 0.0, -1.0 -> schimbat sens
    CPunct p(0.0, 2.5); //orig: 0.0, -1.0 -> schimbat origine

    v.deseneaza(p, 0.25);
    p = v.getDest(p, 0.25);
    arborePerronInversat(lungime, nivel, 0.4, p, v);
  }
};

class CSierpinskiArrowhead
{
public:
  void sierpinskiArrowhead(double lungime, int nivel, CPunct &p, CVector &v, int d)
  {
    if (nivel == 0) 
    {
    }
    else
    {
      v.rotatie(d * 90);
      sierpinskiArrowhead(lungime, nivel - 1, p, v, -d);

      v.deseneaza(p, lungime);
      p = v.getDest(p, lungime);

      v.rotatie(-d * 90);
      sierpinskiArrowhead(lungime, nivel - 1, p, v, d);

    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v(0.0, 1.0);
    CPunct p(0.0, 0.0);

    sierpinskiArrowhead(lungime, nivel, p, v, 1);
  }
};



// ---------------DISPLAYS------------------------------------

// afisare curba lui Koch "fulg de zapada"
void Display1() {
  CCurbaKoch cck;
  cck.afisare(sqrt(3.0), nivel);

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  char str[80] = "Nivel=";
  displayBitmap(strcat(str, c));

  glRasterPos2d(-1.0,0.9);
  displayBitmap("curba lui Koch");

  nivel++;
}

// afisare arbore binar
void Display2() {
  CArboreBinar cab;
  cab.afisare(1, nivel);

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  char str[80] = "Nivel=";
  displayBitmap(strcat(str, c));

  glRasterPos2d(-1.0,0.9);
  displayBitmap("arbore binar");

  nivel++;
}

// afisare arborele lui Perron
void Display3() {
  CArborePerron cap;

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  char str[80] = "Nivel=";
  displayBitmap(strcat(str, c));

  glRasterPos2d(-1.0,-0.9);
  displayBitmap("arbore Perron");

  glPushMatrix();
  glLoadIdentity();
  glScaled(0.4, 0.4, 1);
  glTranslated(-0.5, -0.5, 0.0);
  cap.afisare(1, nivel);
  glPopMatrix();
  nivel++;
}

// afisare curba lui Hilbert
void Display4() {
  CCurbaHilbert cch;
  cch.afisare(0.05, nivel);

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  char str[80] = "Nivel=";
  displayBitmap(strcat(str, c));

  glRasterPos2d(-1.0,-0.9);

  displayBitmap("curba Hilbert");

  nivel++;
}

//Sierpinski carpet
void Display5() {
  CSierpinskiCarpet csc;
  //csc.afisare(0.05, nivel);


  //char display
  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  char str[80] = "Nivel=";
  displayBitmap(strcat(str, c));

  glRasterPos2d(-1.0,-0.9);

  displayBitmap("Sierpinski carpet");

  glPushMatrix();
  glLoadIdentity();
  glScaled(0.4, 0.4, 1);
  // glTranslated(-0.5, -0.5, 0.0);
  csc.afisare(1, nivel);
  glPopMatrix();

  nivel++;
}

//inverted Perron
void Display6() {
  
  CArborePerronInversat cap;

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  char str[80] = "Nivel=";
  displayBitmap(strcat(str, c));

  glRasterPos2d(-1.0,-0.9);
  displayBitmap("inverted Perron");

  glPushMatrix();
  glLoadIdentity();
  glScaled(0.4, 0.4, 1);
  glTranslated(-0.5, -0.5, 0.0);
  cap.afisare(1, nivel);
  glPopMatrix();
  nivel++;
}

//Sierpinski arrowhead
void Display7() {
    


  //char display
  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  char str[80] = "Nivel=";
  displayBitmap(strcat(str, c));

  glRasterPos2d(-1.0,-0.9);

  displayBitmap("Sierpinski arrowhead");

  nivel++;
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
    case '1':
      glClear(GL_COLOR_BUFFER_BIT);
      Display1();
      break;
    case '2':
      glClear(GL_COLOR_BUFFER_BIT);
      Display2();
      break;
    case '3':
      glClear(GL_COLOR_BUFFER_BIT);
      Display3();
      break;
    case '4':
      glClear(GL_COLOR_BUFFER_BIT);
      Display4();
      break;
    case '5':
      glClear(GL_COLOR_BUFFER_BIT);
      Display5();
      break;
    case '6':
      glClear(GL_COLOR_BUFFER_BIT);
      Display6();
      break;
    case '7':
      glClear(GL_COLOR_BUFFER_BIT);
      Display7();
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
