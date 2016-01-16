/***********************************************************
             CSC418/2504, Fall 2009
  
                 penguin.cpp
                 
       Simple demo program using OpenGL and the glut/glui 
       libraries

  
    Instructions:
        Please read the assignment page to determine 
        exactly what needs to be implemented.  Then read 
        over this file and become acquainted with its 
        design.

        Add source code where it appears appropriate. In
        particular, see lines marked 'TODO'.

        You should not need to change the overall structure
        of the program. However it should be clear what
        your changes do, and you should use sufficient comments
        to explain your code.  While the point of the assignment
        is to draw and animate the character, you will
        also be marked based on your design.

***********************************************************/

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glui.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef _WIN32
#include <unistd.h>
#else
void usleep(unsigned int nanosec)
{
    Sleep(nanosec / 1000);
}
#endif


// *************** GLOBAL VARIABLES *************************


const float PI = 3.14159;

// --------------- USER INTERFACE VARIABLES -----------------

// Window settings
int windowID;               // Glut window ID (for display)
GLUI *glui;                 // Glui window (for controls)
int Win[2];                 // window (x,y) size


// ---------------- ANIMATION VARIABLES ---------------------

// Animation settings
int animate_mode = 0;       // 0 = no anim, 1 = animate
int animation_frame = 0;      // Specify current frame of animation
int animate_mode_move = 0;   // 0 = no move, 1 move


//////////////////////////////////////////////////////
// TODO: Add additional joint parameters here
//////////////////////////////////////////////////////
const float JOINT_RADIUS = 4.0f;
const float EYE_RADIUS = 4.7f;
const float EYEBALL_RADIUS = 3.0f;
// Arm Joint parameters
const float ARM_MIN = -35.0f;
const float ARM_MAX =  35.0f;
float arm_rot = 0.0f;

// Right Leg Joint parameters
const float RIGHT_LEG_MIN = -20.0f;
const float RIGHT_LEG_MAX =  20.0f;
float right_leg_rot = 0.0f;

// Right Foot Joint parameters
const float RIGHT_FOOT_MIN = -20.0f;
const float RIGHT_FOOT_MAX =  20.0f;
float right_foot_rot = 0.0f;

// Right Leg Joint parameters
const float LEFT_LEG_MIN = -20.0f;
const float LEFT_LEG_MAX =  20.0f;
float left_leg_rot = 0.0f;

// Right Foot Joint parameters
const float LEFT_FOOT_MIN = -20.0f;
const float LEFT_FOOT_MAX =  20.0f;
float left_foot_rot = 0.0f;

// Head Joint parameters
const float HEAD_MIN = -4.0f;
const float HEAD_MAX = 10.0f;
float head_rot = 0.0f;

// Mouth Joint parameters
const float MOUTH_MIN = 0.0f;
const float MOUTH_MAX = 7.0f;
float mouth_chew = 0.0f;

//Penguin move
const float MOVE_X_MIN = -300.0f;
const float MOVE_X_MAX = 300.0f;
float move_x = 0.0f;
const float MOVE_Y_MIN = -3.0f;
const float MOVE_Y_MAX = 3.0f;
float move_y = 0.0f;
// Turn penguin head
int turn= 1;
bool check = true;
bool moveable = true;
// ***********  FUNCTION HEADER DECLARATIONS ****************


// Initialization functions
void initGlut(char* winName);
void initGlui();
void initGl();


// Callbacks for handling events in glut
void myReshape(int w, int h);
void animate();
void display(void);

// Callback for handling events in glui
void GLUI_Control(int id);


// Functions to help draw the object
void drawCircle(float radius);
void drawCircleFull(float radius);
void drawPolygon(const float x_coor[], const float y_coor[], int sum);
void drawPolygonFull(const float x_coor[], const float y_coor[], int sum);
// Return the current system clock (in seconds)
double getTime();


// ******************** FUNCTIONS ************************



// main() function
// Initializes the user interface (and any user variables)
// then hands over control to the event handler, which calls 
// display() whenever the GL window needs to be redrawn.
int main(int argc, char** argv)
{

    // Process program arguments
    if(argc != 3) {
        printf("Usage: demo [width] [height]\n");
        printf("Using 300x200 window by default...\n");
        Win[0] = 2*int(MOVE_X_MAX);
        Win[1] = 400;
    } else {
        Win[0] = atoi(argv[1]);
        Win[1] = atoi(argv[2]);
    }


    // Initialize glut, glui, and opengl
    glutInit(&argc, argv);
    initGlut(argv[0]);
    initGlui();
    initGl();

    // Invoke the standard GLUT main event loop
    glutMainLoop();

    return 0;         // never reached
}


// Initialize glut and create a window with the specified caption 
void initGlut(char* winName)
{
    // Set video mode: double-buffered, color, depth-buffered
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Create window
    glutInitWindowPosition (0, 0);
    glutInitWindowSize(Win[0],Win[1]);
    windowID = glutCreateWindow(winName);

    // Setup callback functions to handle events
    glutReshapeFunc(myReshape); // Call myReshape whenever window resized
    glutDisplayFunc(display);   // Call display whenever new frame needed 
}


// Quit button handler.  Called when the "quit" button is pressed.
void quitButton(int)
{
  exit(0);
}

// Animate button handler.  Called when the "animate" checkbox is pressed.
void animateButton(int)
{
  // synchronize variables that GLUT uses
  glui->sync_live();

  animation_frame = 0;
  if(animate_mode == 1) {
    // start animation
    GLUI_Master.set_glutIdleFunc(animate);
  } else {
    // stop animation
    GLUI_Master.set_glutIdleFunc(NULL);
  }
}

//enable to moving the penguin
void moveButton(int)
{
  // synchronize variables that GLUT uses
    if(animate_mode_move == 1){
        moveable = false;
    }
    else{
        moveable = true;
    }
}

// Initialize GLUI and the user interface
void initGlui()
{
    GLUI_Master.set_glutIdleFunc(NULL);

    // Create GLUI window
    glui = GLUI_Master.create_glui("Glui Window", 0, Win[0]+10, 0);


    ///////////////////////////////////////////////////////////
    // TODO: 
    //   Add controls for additional joints here
    ///////////////////////////////////////////////////////////
    // Create a control to rotate head 
    GLUI_Spinner *head_spinner
        = glui->add_spinner("Head", GLUI_SPINNER_FLOAT, &head_rot);
    head_spinner->set_speed(0.1);
    head_spinner->set_float_limits(HEAD_MIN, HEAD_MAX, GLUI_LIMIT_CLAMP);

    // Create a control to move mouth 
    GLUI_Spinner *mouth_spinner
        = glui->add_spinner("Mouth", GLUI_SPINNER_FLOAT, &mouth_chew);
    mouth_spinner->set_speed(0.1);
    mouth_spinner->set_float_limits(MOUTH_MIN, MOUTH_MAX, GLUI_LIMIT_CLAMP);
    // Create a control to rotate arm
    GLUI_Spinner *arm_spinner
        = glui->add_spinner("Arm", GLUI_SPINNER_FLOAT, &arm_rot);
    arm_spinner->set_speed(0.1);
    arm_spinner->set_float_limits(ARM_MIN, ARM_MAX, GLUI_LIMIT_CLAMP);

    // Create a control to rotate right leg
    GLUI_Spinner *right_leg_spinner
        = glui->add_spinner("Leg(right)", GLUI_SPINNER_FLOAT, &right_leg_rot);
    right_leg_spinner->set_speed(0.1);
    right_leg_spinner->set_float_limits(RIGHT_LEG_MIN, RIGHT_LEG_MAX, GLUI_LIMIT_CLAMP);

    // Create a control to rotate left leg
    GLUI_Spinner *left_leg_spinner
        = glui->add_spinner("Leg(left)", GLUI_SPINNER_FLOAT, &left_leg_rot);
    left_leg_spinner->set_speed(0.1);
    left_leg_spinner->set_float_limits(LEFT_LEG_MIN, LEFT_LEG_MAX, GLUI_LIMIT_CLAMP);
    
    // Create a control to rotate right foot
    GLUI_Spinner *right_foot_spinner
        = glui->add_spinner("Foot(right)", GLUI_SPINNER_FLOAT, &right_foot_rot);
    right_foot_spinner->set_speed(0.15);
    right_foot_spinner->set_float_limits(RIGHT_FOOT_MIN, RIGHT_FOOT_MAX, GLUI_LIMIT_CLAMP);

    // Create a control to rotate left foot
    GLUI_Spinner *left_foot_spinner
        = glui->add_spinner("Foot(left)", GLUI_SPINNER_FLOAT, &left_foot_rot);
    left_foot_spinner->set_speed(0.15);
    left_foot_spinner->set_float_limits(LEFT_FOOT_MIN, LEFT_FOOT_MAX, GLUI_LIMIT_CLAMP);
    
    // Create a control to move along x coor
    GLUI_Spinner *move_x_spinner
        = glui->add_spinner("Move(x)", GLUI_SPINNER_FLOAT, &move_x);
    move_x_spinner->set_speed(2.0);
    move_x_spinner->set_float_limits(MOVE_X_MIN, MOVE_X_MAX, GLUI_LIMIT_CLAMP);
    
    // Create a control to move along y coor
    GLUI_Spinner *move_y_spinner
        = glui->add_spinner("Move(y)", GLUI_SPINNER_FLOAT, &move_y);
    move_y_spinner->set_speed(1.0);
    move_y_spinner->set_float_limits(MOVE_Y_MIN, MOVE_Y_MAX, GLUI_LIMIT_CLAMP);


    // Add button to specify animation mode 
    glui->add_separator();
    glui->add_checkbox("Animate", &animate_mode, 0, animateButton);
    // Add buuton to enable move
    glui->add_separator();
    glui->add_checkbox("Stop", &animate_mode_move, 1, moveButton);
    
    // Add "Quit" button
    glui->add_separator();
    glui->add_button("Quit", 0, quitButton);

    // Set the main window to be the "active" window
    glui->set_main_gfx_window(windowID);
}


// Performs most of the OpenGL intialization
void initGl(void)
{
    // glClearColor (red, green, blue, alpha)
    // Ignore the meaning of the 'alpha' value for now
    glClearColor(0.7f,0.7f,0.9f,1.0f);
}




// Callback idle function for animating the scene
void animate()
{
    // Update geometry
    ///////////////////////////////////////////////////////////
    // TODO:
    //   Modify this function animate the character's joints
    //   Note: Nothing should be drawn in this function!  OpenGL drawing
    //   should only happen in the display() callback.
    ///////////////////////////////////////////////////////////
    // animate arm
    const double arm_rot_speed = 0.1;
    double arm_rot_t = (sin(animation_frame*arm_rot_speed) + 1.0) / 2.0;
    arm_rot = arm_rot_t * ARM_MIN + (1 - arm_rot_t) * ARM_MAX;
    // animate right leg
    const double right_leg_rot_speed = 0.1;
    double right_leg_rot_t = (sin(animation_frame*right_leg_rot_speed) + 1.0) / 2.0;
    right_leg_rot = right_leg_rot_t * RIGHT_LEG_MIN + (1 - right_leg_rot_t) * RIGHT_LEG_MAX;
    // animate left leg
    const double left_leg_rot_speed = 0.1;
    double left_leg_rot_t = (sin(animation_frame*left_leg_rot_speed) + 1.0) / 2.0;
    left_leg_rot = left_leg_rot_t * LEFT_LEG_MIN + (1 - left_leg_rot_t) * LEFT_LEG_MAX;
    // animate right foot
    const double right_foot_rot_speed = 0.1;
    double right_foot_rot_t = (sin(animation_frame*right_foot_rot_speed) + 1.0) / 2.0;
    right_foot_rot = right_foot_rot_t * RIGHT_FOOT_MIN + (1 - right_foot_rot_t) * RIGHT_FOOT_MAX;
    // animate left foot
    const double left_foot_rot_speed = 0.1;
    double left_foot_rot_t = (sin(animation_frame*left_foot_rot_speed) + 1.0) / 2.0;
    left_foot_rot = left_foot_rot_t * LEFT_FOOT_MIN + (1 - left_foot_rot_t) * LEFT_FOOT_MAX;
    // animate head
    const double head_rot_speed = 0.1;
    double head_rot_t = (sin(animation_frame*head_rot_speed) + 1.0) / 2.0;
    head_rot = head_rot_t * HEAD_MIN + (1 - head_rot_t) * HEAD_MAX;
    //animate mouth
    const double mouth_chew_speed = 0.1;
    double mouth_chew_t = (sin(animation_frame*mouth_chew_speed) + 1.0) / 2.0;
    mouth_chew = mouth_chew_t * MOUTH_MIN + (1 - mouth_chew_t) * MOUTH_MAX;
    // check if the penguin is able to moive 
    if(moveable){
        //animate x move
        const double move_x_speed = 0.005;
        double move_x_t = (sin(animation_frame*move_x_speed) + 1.0) / 2.0;
        move_x = move_x_t * MOVE_X_MIN + (1 - move_x_t) * MOVE_X_MAX;
        //animate y move
        const double move_y_speed = 0.05;
        double move_y_t = (sin(animation_frame*move_y_speed) + 1.0) / 2.0;
        move_y = move_y_t * MOVE_Y_MIN + (1 - move_y_t) * MOVE_Y_MAX;

    }
    // Update user interface
    glui->sync_live();

    // Tell glut window to update itself.  This will cause the display()
    // callback to be called, which renders the object (once you've written
    // the callback).
    glutSetWindow(windowID);
    glutPostRedisplay();

    // increment the frame number.
    animation_frame++;

    // Wait 50 ms between frames (20 frames per second)
    usleep(50000);
}


// Handles the window being resized by updating the viewport
// and projection matrices
void myReshape(int w, int h)
{
    // Setup projection matrix for new window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-w/2, w/2, -h/2, h/2);

    // Update OpenGL viewport and internal variables
    glViewport(0,0, w,h);
    Win[0] = w;
    Win[1] = h;
}



// display callback
//
// This gets called by the event handler to draw
// the scene, so this is where you need to build
// your scene -- make your changes and additions here.
// All rendering happens in this function.  For Assignment 1,
// updates to geometry should happen in the "animate" function.
void display(void)
{
    // glClearColor (red, green, blue, alpha)
    // Ignore the meaning of the 'alpha' value for now
    glClearColor(0.83f,0.83f,0.9f,1.0f);

    // OK, now clear the screen with the background colour
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

    // Setup the model-view transformation matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    ///////////////////////////////////////////////////////////
    // TODO:
    //   Modify this function draw the scene
    //   This should include function calls to pieces that
    //   apply the appropriate transformation matrice and
    //   render the individual body parts.
    ///////////////////////////////////////////////////////////

    // Draw our hinged object
    // body coordinates
    const float BODY_X_COOR[] = { -1.0/4.0, 3.2/15.0, 3.0/4.0, 1.0/6.0, -1.0/6.0, -1.0/2.0 };
    const float BODY_Y_COOR[] = { 1.0/2.0, 1.0/2.0, -3.0/11.0, -1.0/2.0, -1.0/2.0, -3.0/10.0 };
    const float BODY_WIDTH = 140.0f;
    const float BODY_LENGTH = 200.0f;
    // arm coordinates
    const float ARM_X_COOR[] = { -1.0/2.0, 1.0/1.5, 1.0/10.0, -1.0/4.0};
    const float ARM_Y_COOR[] = { 0.0, 0.0, -1.0, -1.0 };
    const float ARM_LENGTH = 80.0f;
    const float ARM_WIDTH = 40.0f;

    // leg coordiantes
    const float LEG_X_COOR[] = { -1.0/2.0, 1.0/1.5, 1.0/10.0, -1.0/2.0};
    const float LEG_Y_COOR[] = { 0.0, 0.0, -1.0, -1.0 };
    const float LEG_LENGTH = 60.0f;
    const float LEG_WIDTH = 16.0f;

    // feet coordinate
    const float FOOT_X_COOR[] = { -1.0, 0.0, 0.0, -1.0 };
    const float FOOT_Y_COOR[] = { 1.0/2.0, 1.0/3, -1.0/2.0, -1.0/2.0};
    const float FOOT_LENGTH = 13.0f;
    const float FOOT_WIDTH = 40.0f;
    // head coordiantes
    const float HEAD_X_COOR[] = {1.0/2.0, 6.0/16.0, -1.0/16.0, -6.0/16.0, -1.0/2.0};
    const float HEAD_Y_COOR[] = { 0.0, 5.0/6.0, 1.0, 8.0/10.0, 0.0 };
    const float HEAD_LENGTH = 70.0f;
    const float HEAD_WIDTH = 86.0f;
    // up mouth coordiantes
    const float MOUTH_UP_X_COOR[] = {-1.0, -15.0/16.0, -1.0/16.0 , 0.0};
    const float MOUTH_UP_Y_COOR[] = {0.0, 2.0/5.0, 1.0, 0.0};
    const float MOUTH_UP_LENGTH = 10.0f;
    const float MOUTH_UP_WIDTH = 50.0f;
    // bottom mouth coordiantes
    const float MOUTH_DOWN_X_COOR[] = {-1.0, -1.0, 0.0, 0.0};
    const float MOUTH_DOWN_Y_COOR[] = {0.0, 1.0, 1.0, 0.0};
    const float MOUTH_DOWN_LENGTH = 2.0f;
    const float MOUTH_DOWN_WIDTH = 50.0f;
    // Push the current transformation matrix on the stack
    glPushMatrix();
        //move pengiun
        glColor3f(0.0f,0.0f,0.0f);
        glTranslatef(move_x, 0.0, 0.0);
        glTranslatef(0.0, move_y, 0.0); 
        //turn the pengiun
        if(move_x < MOVE_X_MIN + 1.0 && check){
            check = false;
            turn = -1;
        }
        if(move_x > MOVE_X_MAX-1.0 && !check){
            check = true;
            turn = 1;
        }
        glScalef(turn, 1, 1);
        // draw body
        glPushMatrix();
            glScalef(BODY_WIDTH, BODY_LENGTH, 1.0);
            drawPolygon(BODY_X_COOR, BODY_Y_COOR, 6);
        glPopMatrix();
        
        // draw arm and arm joint
        glColor3f(1.0f,1.0f,1.0f);
        glPushMatrix();
            glTranslatef(BODY_WIDTH*(1.0/20.0), BODY_LENGTH*(4.0/20.0), 0.0);
            // Draw arm joint 
            drawCircle(JOINT_RADIUS);
            
            // Rotate arm
            glRotatef(-arm_rot, 0.0, 0.0, 1.0);

            // Draw arm - Push here so the arm scale doesnt affect other components
            glPushMatrix();
            // Translate the arm above the joint
                glTranslatef(0.0, JOINT_RADIUS*1.5, 0.0);
                glScalef(ARM_WIDTH, ARM_LENGTH, 1.0);
                drawPolygon(ARM_X_COOR, ARM_Y_COOR, 4);
            glPopMatrix();  
        glPopMatrix();
        glColor3f(0.0f,0.0f,0.0f);

        // draw left leg and foot
        glPushMatrix();
            // move (staisfied golden section)to bottom left and draw joint
            glTranslatef(BODY_WIDTH/4, -BODY_LENGTH/2.0 + LEG_LENGTH*(1.0-0.618), 0.0);
            // draw left leg joint
            drawCircle(JOINT_RADIUS);
            // Rotate left leg and foot
            glRotatef(-left_leg_rot+10, 0.0, 0.0, 1.0);
            // push the joint translation
            glPushMatrix();
                // move above and draw left leg over the joint
                glTranslatef(0.0, JOINT_RADIUS*1.5, 0.0);
                // Draw left leg and push pop matrix
                glPushMatrix();
                    glScalef(LEG_WIDTH, LEG_LENGTH, 1.0);
                    drawPolygon(LEG_X_COOR, LEG_Y_COOR, 4);
                glPopMatrix();
                // left foot
                glPushMatrix();                    
                    // move to the bottom of left leg 
                    glTranslatef(0.0, -LEG_LENGTH + JOINT_RADIUS*1.5, 0.0);
                    // draw the left foot joint
                    glColor3f(1.0f,1.0f,1.0f); 
                    drawCircle(JOINT_RADIUS);
                    // rotate left foot
                    glRotatef(left_foot_rot, 0.0, 0.0, 1.0);            
                    glPushMatrix();
                        // move left foot over joint 
                        glTranslatef(JOINT_RADIUS*1.5, 0.0, 0.0);    
                        // Draw left foot
                        glPushMatrix();
                            glScalef(FOOT_WIDTH, FOOT_LENGTH, 1.0);
                            drawPolygon(FOOT_X_COOR, FOOT_Y_COOR, 4);
                            glColor3f(0.0f,0.0f,0.0f);
                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();

        // draw right leg and foot
        glPushMatrix();
            // move (staisfied golden section)to bottom right and draw joint 
            // minus position compare to left leg
            glTranslatef(-BODY_WIDTH/4, -BODY_LENGTH/2.0 + LEG_LENGTH*(1.0-0.618), 0.0);
            // draw right leg joint
            drawCircle(JOINT_RADIUS);
            // Rotate rightleg and foot go forward
            glRotatef(right_leg_rot-10, 0.0, 0.0, 1.0);
            // push the joint translation
            glPushMatrix();
                // move above and draw left leg over the joint
                glTranslatef(0.0, JOINT_RADIUS*1.5, 0.0);
                // Draw left leg and push pop matrix
                glPushMatrix();
                    glScalef(LEG_WIDTH, LEG_LENGTH, 1.0);
                    drawPolygon(LEG_X_COOR, LEG_Y_COOR, 4);
                glPopMatrix();
                // left foot
                glPushMatrix();                    
                    // move to the bottom of left leg 
                    glTranslatef(0.0, -LEG_LENGTH + JOINT_RADIUS*1.5, 0.0);
                    // draw the left foot joint
                    glColor3f(1.0f,1.0f,1.0f); 
                    drawCircle(JOINT_RADIUS);
                    // rotate right foot opposite dirction with leg
                    glRotatef(-right_foot_rot+13, 0.0, 0.0, 1.0);            
                    glPushMatrix();
                        // move right foot over joint 
                        glTranslatef(JOINT_RADIUS*1.5, 0.0, 0.0);    
                        // Draw right foot
                        glPushMatrix();
                            glScalef(FOOT_WIDTH, FOOT_LENGTH, 1.0);
                            drawPolygon(FOOT_X_COOR, FOOT_Y_COOR, 4);
                            glColor3f(0.0f,0.0f,0.0f);
                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();

        //draw head 
        glColor3f(0.0f,0.0f,0.0f);
        glPushMatrix();
            // move to top and draw head joint
            glTranslatef(0.0, BODY_LENGTH/2 - JOINT_RADIUS*2, 0.0);
            //draw joint
            drawCircle(JOINT_RADIUS);
            // draw head over joint
            glTranslatef(-1.5, -JOINT_RADIUS*1.5, 0.0);
            glRotatef(head_rot, 0.0, 0.0, 1.0); 
            // push pop head scale matrix
            glPushMatrix();
                glScalef(HEAD_WIDTH, HEAD_LENGTH, 1.0);
                drawPolygon(HEAD_X_COOR, HEAD_Y_COOR, 5);
            glPopMatrix();

            //move to draw eye
            glPushMatrix();
                glTranslatef(-HEAD_WIDTH*2.0/8.0, HEAD_LENGTH*5.0/8.0, 0.0);
                glColor3f(1.0f,1.0f,1.0f);
                drawCircleFull(EYE_RADIUS);
                glTranslatef(EYEBALL_RADIUS-EYE_RADIUS, 0.0, 0.0);
                glColor3f(0.0f,0.0f,0.0f);
                drawCircleFull(EYEBALL_RADIUS);
            glPopMatrix();
            glColor3f(1.0f,1.0f,1.0f);
            // move to draw upper beak
            glTranslatef(-HEAD_WIDTH/2.5, HEAD_LENGTH/2.5, 0.0);

            glPushMatrix();
                glScalef(MOUTH_UP_WIDTH, MOUTH_UP_LENGTH, 1.0);
                drawPolygon(MOUTH_UP_X_COOR, MOUTH_UP_Y_COOR, 4);
            glPopMatrix();

            glTranslatef(0.0, -3.5*MOUTH_DOWN_LENGTH, 0.0);
            // move down to draw bottom beak
            glPushMatrix();
                //chewing the mouth
                glTranslatef(0.0, mouth_chew, 0.0);
                //draw mouth bottom
                glColor3f(0.0f,0.0f,0.0f);
                glScalef(MOUTH_DOWN_WIDTH, MOUTH_DOWN_LENGTH, 1.0);
                drawPolygonFull(MOUTH_DOWN_X_COOR, MOUTH_DOWN_Y_COOR, 4);
                glColor3f(1.0f,1.0f,1.0f);
            glPopMatrix();
        glPopMatrix();
    // Retrieve the previous state of the transformation stack
    glPopMatrix();

    // Execute any GL functions that are in the queue just to be safe
    glFlush();

    // Now, show the frame buffer that we just drew into.
    // (this prevents flickering).
    glutSwapBuffers();
}
// draw circle with given radius
void drawCircle(float radius)
{
    glBegin(GL_LINE_LOOP);
        for( float angle = 0; angle < 2*PI; angle += (PI/180) )
        glVertex2f( radius*sin(angle), radius*cos(angle));
    glEnd();
}

// Draw a polgon with givn coordinates
void drawPolygon(const float xcoordnates[], const float ycoordnates[], int sum)
{
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < sum; i += 1) {
            glVertex2d(xcoordnates[i], ycoordnates[i]);
        }
    glEnd();
}

// draw circle with given radius full color
void drawCircleFull(float radius)
{
    glBegin(GL_POLYGON);
        for( float angle = 0; angle < 2*PI; angle += (PI/180) )
        glVertex2f( radius*sin(angle), radius*cos(angle));
    glEnd();
}
// Draw a polgon with givn coordinates full color
void drawPolygonFull(const float xcoordnates[], const float ycoordnates[], int sum)
{
    glBegin(GL_POLYGON);
        for (int i = 0; i < sum; i += 1) {
            glVertex2d(xcoordnates[i], ycoordnates[i]);
        }
    glEnd();
}


