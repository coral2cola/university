import glfw
from OpenGL.GL import *
import numpy as np

i = 4

def render(n):
    glClear(GL_COLOR_BUFFER_BIT)
    glLoadIdentity()

    if n == 1:
        glBegin(GL_POINTS)
    elif n == 2:
        glBegin(GL_LINES)
    elif n == 3:
        glBegin(GL_LINE_STRIP)
    elif n == 4:
        glBegin(GL_LINE_LOOP)
    elif n == 5:
        glBegin(GL_TRIANGLES)
    elif n == 6:
        glBegin(GL_TRIANGLE_STRIP)
    elif n == 7:
        glBegin(GL_TRIANGLE_FAN)
    elif n == 8:
        glBegin(GL_QUADS)
    elif n == 9:
        glBegin(GL_QUAD_STRIP)
    elif n == 0:
        glBegin(GL_POLYGON)
    else:
        glBegin(GL_LINE_LOOP)

    glColor3ub(255, 255, 255)
    
    th = np.linspace(0, np.pi*2, 13)

    glVertex2fv(np.array([np.cos(th[0]), np.sin(th[0])]))
    glVertex2fv(np.array([np.cos(th[1]), np.sin(th[1])]))
    glVertex2fv(np.array([np.cos(th[2]), np.sin(th[2])]))
    glVertex2fv(np.array([np.cos(th[3]), np.sin(th[3])]))
    glVertex2fv(np.array([np.cos(th[4]), np.sin(th[4])]))
    glVertex2fv(np.array([np.cos(th[5]), np.sin(th[5])]))
    glVertex2fv(np.array([np.cos(th[6]), np.sin(th[6])]))
    glVertex2fv(np.array([np.cos(th[7]), np.sin(th[7])]))
    glVertex2fv(np.array([np.cos(th[8]), np.sin(th[8])]))
    glVertex2fv(np.array([np.cos(th[9]), np.sin(th[9])]))
    glVertex2fv(np.array([np.cos(th[10]), np.sin(th[10])]))
    glVertex2fv(np.array([np.cos(th[11]), np.sin(th[11])]))
     
    glEnd()


def key_callback(window, key, scancode, action, mods):
    global i
    if key == glfw.KEY_0:
        if action == glfw.PRESS:
            i = 0
    elif key == glfw.KEY_1:
        if action == glfw.PRESS:
            i = 1
    elif key == glfw.KEY_2:
        if action == glfw.PRESS:
            i = 2
    elif key == glfw.KEY_3:
        if action == glfw.PRESS:
            i = 3
    elif key == glfw.KEY_4:
        if action == glfw.PRESS:
            i = 4
    elif key == glfw.KEY_5:
        if action == glfw.PRESS:
            i = 5     
    elif key == glfw.KEY_6:
        if action == glfw.PRESS:
            i = 6
    elif key == glfw.KEY_7:
        if action == glfw.PRESS:
            i = 7
    elif key == glfw.KEY_8:
        if action == glfw.PRESS:
            i = 8
    elif key == glfw.KEY_9:
        if action == glfw.PRESS:
            i = 9
    else:
        i = 4

def main():
    global i
    if not glfw.init():
        return
    window = glfw.create_window(480,480,"2018008531-3-1",None,None)
    if not window:
        glfw.terminate()
        return

    glfw.set_key_callback(window, key_callback)
    glfw.make_context_current(window)


    while not glfw.window_should_close(window):
        glfw.poll_events()

        render(i)
        glfw.swap_buffers(window)

    glfw.terminate()

if __name__ == "__main__":
    main()