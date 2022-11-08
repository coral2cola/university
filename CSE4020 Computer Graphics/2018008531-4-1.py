import glfw
from OpenGL.GL import *
import numpy as np

#global variable to store current accumulated transformation
gComposedM = np.array([[1.,0.,0.],
                       [0.,1.,0.],
                       [0.,0.,1.]])


def render(T):
    glClear(GL_COLOR_BUFFER_BIT)
    glLoadIdentity()
    # draw cooridnate
    glBegin(GL_LINES)
    glColor3ub(255, 0, 0)
    glVertex2fv(np.array([0.,0.]))
    glVertex2fv(np.array([1.,0.]))
    glColor3ub(0, 255, 0)
    glVertex2fv(np.array([0.,0.]))
    glVertex2fv(np.array([0.,1.]))
    glEnd()
    # draw triangle
    glBegin(GL_TRIANGLES)
    glColor3ub(255, 255, 255)
    glVertex2fv( (T @ np.array([.0,.5,1.]))[:-1] )
    glVertex2fv( (T @ np.array([.0,.0,1.]))[:-1] )
    glVertex2fv( (T @ np.array([.5,.0,1.]))[:-1] )
    glEnd()

def key_callback(windodw, key, scancode, action, mods):
    global gComposedM
    if action == glfw.PRESS or action == glfw.REPEAT:
        if key == glfw.KEY_Q:
            #Translate by -0.1 in x direction w.r.t global coordinate
            newM = np.array([[1.,0.,-0.1],
                            [0.,1.,0.],
                            [0.,0.,1.]])

            #local coordinate
            #gComposedM = gComposedM @ newM

            #global coordinate
            gComposedM = newM @ gComposedM 

        elif key == glfw.KEY_E:
            #Translate by 0.1 in x direction w.r.t global coordinate
            newM = np.array([[1.,0.,0.1],
                            [0.,1.,0.],
                            [0.,0.,1.]])
            gComposedM = newM @gComposedM

        elif key == glfw.KEY_A:
            #Rotate by 10 degrees counterclockwise w.r.t local coordinate
            th = np.pi/18 #10 degrees
            newM = np.array([[np.cos(th),-np.sin(th),0.],
                             [np.sin(th),np.cos(th),0.],
                             [0.,0.,1.]])
            gComposedM = gComposedM @ newM

        elif key == glfw.KEY_D:
            #Rotate by 10 degrees clockwise w.r.t local coordinate
            th = -np.pi/18 #10 degrees
            newM = np.array([[np.cos(th),-np.sin(th),0.],
                             [np.sin(th),np.cos(th),0.],
                             [0.,0.,1.]])
            gComposedM = gComposedM @ newM

        elif key == glfw.KEY_1:
            #Reset the triangle with identity matrix
            gComposedM = np.array([[1.,0.,0.],
                                   [0.,1.,0.],
                                   [0.,0.,1.]])

        elif key == glfw.KEY_W:
            #Scale by 0.9 times in x direction w.r.t global coordinate
            newM = np.array([[0.9,0.,0.],
                             [0.,1.,0.],
                             [0.,0.,1.]])
            gComposedM = newM @ gComposedM

        elif key == glfw.KEY_S:
            #Rotate by 10 degrees counterclockwise w.r.t global coordinate
            th = np.pi/18 #10 degrees
            newM = np.array([[np.cos(th),-np.sin(th),0.],
                             [np.sin(th),np.cos(th),0.],
                             [0.,0.,1.]])
            gComposedM = newM @ gComposedM

def main():
    if not glfw.init():
        return

    window = glfw.create_window(480,480,"2018008531-4-1",None,None)

    if not window:
        glfw.terminate()
        return

    glfw.make_context_current(window)
    glfw.set_key_callback(window, key_callback)

    while not glfw.window_should_close(window):
        glfw.poll_events()
        render(gComposedM)
        glfw.swap_buffers(window)
    glfw.terminate()

if __name__ == "__main__":
    main()