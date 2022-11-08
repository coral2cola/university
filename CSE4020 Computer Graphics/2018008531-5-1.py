import glfw
from OpenGL.GL import *
from OpenGL.GLU import *
import numpy as np

def drawFrame():
    glBegin(GL_LINES)
    glColor3ub(255, 0, 0)
    glVertex3fv(np.array([0.,0.,0.]))
    glVertex3fv(np.array([1.,0.,0.]))
    glColor3ub(0, 255, 0)
    glVertex3fv(np.array([0.,0.,0.]))
    glVertex3fv(np.array([0.,1.,0.]))
    glColor3ub(0, 0, 255)
    glVertex3fv(np.array([0.,0.,0]))
    glVertex3fv(np.array([0.,0.,1.]))
    glEnd()

def drawUnitCube():
    glBegin(GL_QUADS)
    glVertex3f( 0.5, 0.5,-0.5)
    glVertex3f(-0.5, 0.5,-0.5)
    glVertex3f(-0.5, 0.5, 0.5)
    glVertex3f( 0.5, 0.5, 0.5)
    glVertex3f( 0.5,-0.5, 0.5)
    glVertex3f(-0.5,-0.5, 0.5)
    glVertex3f(-0.5,-0.5,-0.5)
    glVertex3f( 0.5,-0.5,-0.5)
    glVertex3f( 0.5, 0.5, 0.5)
    glVertex3f(-0.5, 0.5, 0.5)
    glVertex3f(-0.5,-0.5, 0.5)
    glVertex3f( 0.5,-0.5, 0.5)
    glVertex3f( 0.5,-0.5,-0.5)
    glVertex3f(-0.5,-0.5,-0.5)
    glVertex3f(-0.5, 0.5,-0.5)
    glVertex3f( 0.5, 0.5,-0.5)
    glEnd()


def drawCubeArray():
    for i in range(5):
        for j in range(5):
            for k in range(5):
                glPushMatrix()
                glTranslatef(i,j,-k-1)
                glScalef(.5,.5,.5)
                drawUnitCube()
                glPopMatrix()


def render():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE )
    glLoadIdentity()

    myOrtho(-5,5, -5,5, -8,8)
    myLookAt(np.array([5,3,5]), np.array([1,1,-1]), np.array([0,1,0]))

    # Above two lines must behaves exactly same as the below two lines
    #glOrtho(-5,5, -5,5, -8,8)
    #gluLookAt(5,3,5, 1,1,-1, 0,1,0)


    drawFrame()

    glColor3ub(255, 255, 255)
    drawCubeArray()

# 2 norm of v : ||v|| = np.sqrt( np.dot(v, v) )
# a x b (cross product) : np.cross(a, b)
# a ∙ b (inner product) : np.dot(a, b) or a@b
# Use glMultMatrixf() to multiply your projection matrix and viewing matrix
# to the current transformation matrix.

def myOrtho(left, right, bottom, top, near, far):

    tx = (right + left) / (right - left)
    ty = (top + bottom) / (top - bottom)
    tz = (far + near) / (far - near)

    x = np.array([(2/(right - left)), .0, .0, -tx])
    y = np.array([.0, (2/(top - bottom)), .0, -ty])
    z = np.array([.0, .0, (-2/(far - near)), -tz])

    M = np.array([x, y, z, np.array([.0, .0, .0, 1.])])

    glMultMatrixf(M)


def myLookAt(eye, at, up):

    n = (at - eye)
    n = n / np.sqrt( np.dot(n, n))

    a = up @ n
    v = up - a * n
    v = v / np.sqrt(v@v)

    w = np.cross(n, v)

    #(w, v, -n)

    w = np.append(w, np.array([0.0]))
    v = np.append(v, np.array([0.0]))
    n = np.append(-n, np.array([0.0]))
    

    M = np.array([w, v, n, np.array([0.0,0.0,0.0,1.0])])
    M = M.T

    glMultMatrixf(M)
    glTranslatef(-eye[0], -eye[1], -eye[2])



def main():
    if not glfw.init():
        return

    window = glfw.create_window(480,480,"2018008531-5-1",None,None)

    if not window:
        glfw.terminate()
        return

    glfw.make_context_current(window)

    while not glfw.window_should_close(window):
        glfw.poll_events()
        render()
        glfw.swap_buffers(window)
    glfw.terminate()

if __name__ == "__main__":
    main()