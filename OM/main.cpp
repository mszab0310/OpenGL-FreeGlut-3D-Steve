#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
#include <freeglut.h>
#define MESH 10

GLfloat light_diffuse_obejct[] = { 1.0, 1.0, 0.0, 1.0 }; /* Red diffuse light. */
GLfloat light_diffuse_skin[] = { 0.8, 0.7, 0.6, 1.0 }; /* Red diffuse light. */
GLfloat light_diffuse_blue[] = { 0.0, 0.0, 1.0, 1.0 }; /* Red diffuse light. */
GLfloat light_diffuse_red[] = { 1.0, 0.0, 0.0, 1.0 }; /* Red diffuse light. */
GLfloat light_diffuse_black [] = { 0.0, 0.0, 0.0, 1.0 }; /* Red diffuse light. */
GLfloat light_diffuse_mesh[] = { 1.0, 1.0, 1.0, 1.0 }; /* White diffuse light. rgba */
GLfloat light_position[] = { 1.0, 1.0, -0.5, 0.0 }; /* Infinite light location. */
const int SUBSCT = 4;

struct mouse_input_t
{
    int buttons[3];
    int x, y;
} mouse;

/* Camera vectors */
struct Vector3d
{
    float x, y, z;

} rot, eye;

void crossProduct(float* c, float a[3], float b[3]) // finds the cross product of two vectors
{
    c[0] = a[1] * b[2] - b[1] * a[2];
    c[1] = a[2] * b[0] - b[2] * a[0];
    c[2] = a[0] * b[1] - b[0] * a[1];
}
void normalize(float* vect) //scales a vector a length of 1
{
    float length;
    int i;
    length = sqrt( //A^2 + B^2 + C^2 = length^2
        pow(vect[0], 2) +
        pow(vect[1], 2) +
        pow(vect[2], 2)
    );
    for (i = 0; i < 3; ++i) //divides vector by its length to normalise
    {
        vect[i] /= length;
    }
}
void getFaceNormal(GLfloat* norm, float pointa[3], float pointb[3], float pointc[3])
{
    float vect[2][3];
    int i, j;
    float point[3][3];
    for (i = 0; i < 3; ++i)
    {
        point[0][i] = pointa[i]; //copies points into point[][]
        point[1][i] = pointb[i];
        point[2][i] = pointc[i];
    }
    for (i = 0; i < 2; ++i) //calculates vectors from point[0] to point[1]
    {
        //and point[0] to point[2]
        for (j = 0; j < 3; ++j)
        {
            vect[i][j] = point[2 - i][j] - point[0][j];
        }
    }
    crossProduct(norm, vect[0], vect[1]); //calculates vector at 90° to to 2 vectors
    normalize(norm); //makes the vector length 1
}

void DrawGrid(int HALF_GRID_SIZE)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, light_diffuse_mesh);
    glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, light_diffuse_mesh);
    glBegin(GL_LINES);
    for (int i = -HALF_GRID_SIZE; i <= HALF_GRID_SIZE; i++)
    {
        glColor3f(0, 1, 0);
        glVertex3f((float)i, 0, (float)-HALF_GRID_SIZE);
        glVertex3f((float)i, 0, (float)HALF_GRID_SIZE);

        glVertex3f((float)-HALF_GRID_SIZE, 0, (float)i);
        glVertex3f((float)HALF_GRID_SIZE, 0, (float)i);
    }
    glEnd();
}


void drawCube(float width, float height, GLfloat *light) {
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, light);
    glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, light);

    int i;
    GLfloat A[SUBSCT][3];
    GLfloat B[SUBSCT][3];
    GLfloat C[SUBSCT][3];
    GLfloat D[SUBSCT][3];
    GLfloat N[3] = { 0.0, 0.0, 0.0 };
    float aWidth = width * 0.15f;
    float aDepth = height * (-0.15f);
    float bWidth = width * 0.15f;
    float bDepth = height * 0.025f;
    float cWidth = width * 0.15f;
    float cDepth = height * 0.2f;
    float dWidth = width * 0.15f;
    float dDepth = -height * 0.15f;
    //dimensinunle pentru compnonentele coloanei
    for (int i = 0; i < SUBSCT; i++)
    {
        A[i][0] = aWidth * cos(i * 2 * 3.1415 / (SUBSCT));
        A[i][1] = aWidth * sin(i * 2 * 3.1415 / (SUBSCT));
        A[i][2] = aDepth;
    }
    for (int i = 0; i < SUBSCT; i++)
    {
        B[i][0] = bWidth * cos(i * 2 * 3.1415 / (SUBSCT));
        B[i][1] = bWidth * sin(i * 2 * 3.1415 / (SUBSCT));
        B[i][2] = bDepth;
    }
    for (int i = 0; i < SUBSCT; i++)
    {
        C[i][0] = cWidth * cos(i * 2 * 3.1415 / (SUBSCT));
        C[i][1] = cWidth * sin(i * 2 * 3.1415 / (SUBSCT));
        C[i][2] = cDepth;
    }

    for (int i = 0; i < SUBSCT; i++)
    {
        D[i][0] = dWidth * cos(i * 2 * 3.1415 / (SUBSCT));
        D[i][1] = dWidth * sin(i * 2 * 3.1415 / (SUBSCT));
        D[i][2] = dDepth;
    }

    for (i = 0; i < SUBSCT; i++)
    {
        glBegin(GL_TRIANGLES);
        int j = (i + 1) % (SUBSCT);
        getFaceNormal(&N[0], A[i], B[i], A[j]);
        glNormal3fv(&N[0]);
        glVertex3fv(&A[i][0]);
        glVertex3fv(&B[i][0]);
        glVertex3fv(&A[j][0]);
        // next triangle
        getFaceNormal(&N[0], B[i], B[j], A[j]);
        glNormal3fv(&N[0]);
        glVertex3fv(&B[i][0]);
        glVertex3fv(&B[j][0]);
        glVertex3fv(&A[j][0]);
        glEnd();
    }
    for (i = 0; i < SUBSCT; i++)
    {
        glBegin(GL_TRIANGLES);
        int j = (i + 1) % (SUBSCT);
        getFaceNormal(&N[0], B[i], C[i], B[j]);
        glNormal3fv(&N[0]);
        glVertex3fv(&B[i][0]);
        glVertex3fv(&C[i][0]);
        glVertex3fv(&B[j][0]);
        // next triangle
        getFaceNormal(&N[0], C[i], C[j], B[j]);
        glNormal3fv(&N[0]);
        glVertex3fv(&C[i][0]);
        glVertex3fv(&C[j][0]);
        glVertex3fv(&B[j][0]);
        glEnd();
    }
    for (i = 0; i < SUBSCT; i++)
    {
        glBegin(GL_TRIANGLES);
        int j = (i + 1) % (SUBSCT);
        getFaceNormal(&N[0], C[i], D[i], C[j]);
        glNormal3fv(&N[0]);
        glVertex3fv(&C[i][0]);
        glVertex3fv(&D[i][0]);
        glVertex3fv(&C[j][0]);
        // next triangle
        getFaceNormal(&N[0], D[i], D[j], C[j]);
        glNormal3fv(&N[0]);
        glVertex3fv(&D[i][0]);
        glVertex3fv(&D[j][0]);
        glVertex3fv(&C[j][0]);
        glEnd();
    }
    glNormal3f(0., 0., 1.);
    glBegin(GL_POLYGON);
    for (int i = SUBSCT - 1; i >= 0; i--)
        glVertex3fv(&C[i][0]);
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = SUBSCT - 1; i >= 0; i--)
        glVertex3fv(&D[i][0]);
    glEnd();
    glFlush();    
}

void drawBody(int width, int height) {
    
    drawCube(width, height,light_diffuse_red);
    
}

void drawHands(float shoulderW, float shoulderH, float foreArmW, float foreArmH) {

    glPushMatrix();
    glRotatef(90, 1, 1, 0);
    glTranslatef(0, 0, 1.08); //to align shoulders with sides of the torso
    drawCube(shoulderW, shoulderH,light_diffuse_blue);
    glTranslatef(0, 0, 1.05); //to align forearm with sides of the elbow
    drawCube(foreArmW, foreArmH,light_diffuse_skin);
    glPopMatrix();
}

void drawLeg(float thighW, float thighH, float calfW, float calfH) {
    glPushMatrix();
    glRotatef(180, 1, 0, 0);
    glTranslatef(0, 0,1.75);
    glTranslatef(0.25, 0.25, 0);
    drawCube(thighW, thighH,light_diffuse_blue);
    glTranslatef(0, 0, 1.2);
    drawCube(calfW, calfH,light_diffuse_skin);
    glPopMatrix();
}


void drawBothHands(float shoulderW, float shoulderH, float foreArmW, float foreArmH) {
    glPushMatrix();
    glTranslatef(0,0,1);
    drawHands(shoulderW, shoulderH, foreArmW, foreArmH); // right hand
    //because we rotated 9 degree on xy axis, we flipped direction, so in order to mirror hand to the other side, we rotate 180 on z 
    glRotatef(180, 0, 0, 1);
    drawHands(shoulderW, shoulderH, foreArmW, foreArmH);
    glPopMatrix();
}

void drawHead(float headW, float headH) {
    glPushMatrix();
    glTranslatef(0, 0, 1.85);
    drawCube(headW, headH,light_diffuse_skin);
    glTranslatef(0.1, 0.35, 0.2);
    drawCube(0.5, 0.25, light_diffuse_black);
    glTranslatef(0.25,-0.25,0);
    drawCube(0.5, 0.25, light_diffuse_black);
    glPopMatrix();
}

void drawBothLegs(float thighW, float thighH, float calfW, float calfH) {
    drawLeg(thighW, thighH, calfW, calfH); //left leg
    glPushMatrix();
    glTranslatef(-0.5, 0.5, 0);
    drawLeg(thighW, thighH, calfW, calfH); //right leg
    glPopMatrix();
}

void
display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    /* Camera rotation */
    glTranslated(-eye.x, -eye.y, -eye.z);
    glRotated(rot.x, 1.0f, 0.0f, 0.0f);
    glRotated(rot.y, 0.0f, 1.0f, 0.0f);
    glRotated(rot.z, 0.0f, 0.0f, 1.0f);
    //HERE YOU CALL  YOUR CUSTOM DRAWING FUNCTIONS OR USE BUILT IN ONES
    glPushMatrix(); //save current matrix
    //DrawGrid(MESH);
    glRotatef(45, 0, 0, 1);
    drawHead(3, 2);
    drawBody(6,8);
    drawBothHands(3,3, 2,3);
    drawBothLegs(2.75, 4, 2, 3);
    glPopMatrix();//restore current matrix
    //not really needed if u did push and pop but we translated&rotated a couple of times so whatever

    glutSwapBuffers();
}

static void
reshape(int w, int h)
{
    if (h == 0)
        h = 1;

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /*
        fovy - Specifies the field of view angle, in degrees, in the y direction.
        aspect - Specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).
        zNear - Specifies the distance from the viewer to the near clipping plane (always positive).
        zFar - Specifies the distance from the viewer to the far clipping plane (always positive).
        */
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.1, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutPostRedisplay();
}

void
init(void)
{
    /* Init mouse */
    mouse.buttons[GLUT_LEFT_BUTTON] = GLUT_UP;
    mouse.buttons[GLUT_MIDDLE_BUTTON] = GLUT_UP;
    mouse.buttons[GLUT_RIGHT_BUTTON] = GLUT_UP;
    mouse.x = 0;
    mouse.y = 0;

    /* Init camera input */
    rot.x = 7.0f;
    eye.x = 0.0f;
    rot.y = 0.0f;
    eye.y = 0.0f;
    rot.z = 0.0f;
    eye.z = 7.0f;

    /* Enable a single OpenGL light. */
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    /* since back "face" appears in wireframe mode */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    /* Setup the view of the cube. */
    glMatrixMode(GL_PROJECTION);
    gluPerspective( /* field of view in degree */ 40.0,
        /* aspect ratio */ 1.0,
        /* Z near */ 1.0, /* Z far */ 10.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 0.0, 5.0, /* eye is at (0,0,5) */
        0.0, 0.0, 0.0, /* center is at (0,0,0) */
        0.0, 1.0, 0.); /* up is in positive Y direction */

/* Adjust cube position to be asthetic angle. */
    glTranslatef(0.0, 0.0, -1.0);
    glRotatef(60, 1.0, 0.0, 0.0);
}

static void
mouseMotion(int x, int y)
{
    if (mouse.buttons[GLUT_RIGHT_BUTTON] == GLUT_DOWN)
    {
        /* Zoom */
        eye.z += (x - mouse.x) * 0.1;
    }
    else if (mouse.buttons[GLUT_LEFT_BUTTON] == GLUT_DOWN)
    {
        /* Rotation */
        rot.x += (y - mouse.y);
        rot.y += (x - mouse.x);
    }

    mouse.x = x;
    mouse.y = y;

    glutPostRedisplay();
}

/**
 * GLUT's mouse button callback function.
 * Called when the user press a mouse button. Update mouse
 * state and keyboard modifiers.
 */
static void
mouseButton(int button, int state, int x, int y)
{
    /* Update mouse state */
    mouse.buttons[button] = state;
    mouse.x = x;
    mouse.y = y;
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("Steve Minecraft");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseButton);
    init();
    glutMainLoop();
    return 0;
}
