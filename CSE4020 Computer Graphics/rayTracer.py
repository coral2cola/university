#!/usr/bin/env python3
# -*- coding: utf-8 -*
# sample_python aims to allow seamless integration with lua.
# see examples below

import os
import sys
import pdb  # use pdb.set_trace() for debugging
import code # or use code.interact(local=dict(globals(), **locals()))  for debugging.
import xml.etree.ElementTree as ET
import numpy as np
from PIL import Image

LIMIT = sys.maxsize

class Color:
    def __init__(self, R, G, B):
        self.color=np.array([R,G,B]).astype(float)

    # Gamma corrects this color.
    # @param gamma the gamma value to use (2.2 is generally used).
    def gammaCorrect(self, gamma):
        inverseGamma = 1.0 / gamma
        self.color=np.power(self.color, inverseGamma)

    def toUINT8(self):
        return (np.clip(self.color, 0,1)*255).astype(np.uint8)


class Shader:
    def __init__(self, type):
        self.type = type


class Phong(Shader):
    def __init__(self, diffuse, specular, exponent):
        self.diffuse = diffuse
        self.specular = specular
        self.exponent = exponent


class Lambertian(Shader):
    def __init__(self, diffuse):
        self.diffuse = diffuse


class Sphere:
    def __init__(self, shader, center, radius):
        self.shader = shader
        self.center = center
        self.radius = radius


class Box:
    def __init__(self, shader, minPt, maxPt, normal):
        self.shader = shader
        self.minPt = minPt
        self.maxPt = maxPt
        self.normal = normal


class Light:
    def __init__(self, position, intensity):
        self.position = position
        self.intensity = intensity


def raytrace(list, ray, viewPoint):
    global LIMIT
    m = LIMIT

    index = -1
    count = 0

    for i in list:
        if i.__class__.__name__ == 'Sphere':


            a = np.sum(ray * ray)
            b = np.sum((viewPoint - i.center) * ray)
            c = np.sum((viewPoint - i.center) ** 2) - i.radius ** 2

            check = b ** 2 - a * c

            if check >= 0:
                if -b + np.sqrt(check) >= 0:
                    if m >= (-b + np.sqrt(check)) / a:
                        m = (-b + np.sqrt(check)) / a
                        index = count

                if -b - np.sqrt(check) >= 0:
                    if m >= (-b - np.sqrt(check)) / a:
                        m = (-b - np.sqrt(check)) / a
                        index = count


        elif i.__class__.__name__ == 'Box':

            result = 'true'

            invdirx = 1 / ray[0]
            if(invdirx >= 0):
                tmin = (i.minPt[0] - viewPoint[0]) * invdirx
                tmax = (i.maxPt[0] - viewPoint[0]) * invdirx
            else:
                tmin = (i.maxPt[0] - viewPoint[0]) * invdirx
                tmax = (i.minPt[0] - viewPoint[0]) * invdirx

            invdiry = 1 / ray[1]
            if(invdiry >= 0):
                tymin = (i.minPt[1] - viewPoint[1]) * invdiry
                tymax = (i.maxPt[1] - viewPoint[1]) * invdiry
            else:
                tymin = (i.maxPt[1] - viewPoint[1]) * invdiry
                tymax = (i.minPt[1] - viewPoint[1]) * invdiry

            invdirz = 1 / ray[2]
            if(invdirz >= 0):
                tzmin = (i.minPt[2] - viewPoint[2]) * invdirz
                tzmax = (i.maxPt[2] - viewPoint[2]) * invdirz
            else:
                tzmin = (i.maxPt[2] - viewPoint[2]) * invdirz
                tzmax = (i.minPt[2] - viewPoint[2]) * invdirz
            
            if((tmin > tymax) or (tymin > tmax)):
                result = 'false'

            if(tymin > tmin):
                tmin = tymin
            
            if(tymax < tmax):
                tmax = tymax

            if((tmin > tzmax) or (tzmin > tmax)):
                result = 'false'

            if(tzmin > tmin):
                tmin = tzmin
            
            if(tzmax < tmax):
                tmax = tzmax

            if result == 'true':
                if m >= tmin:
                    m = tmin
                    index = count

        count = count + 1

    return [m, index]


def shade(m, ray, viewPoint, list, idx, light):
    if idx == -1:
        return np.array([0, 0, 0])
    else:
        x = 0
        y = 0
        z = 0
        n = np.array([0, 0, 0])
        v = -m*ray

        if list[idx].__class__.__name__ == 'Sphere':
            n = viewPoint + m*ray - list[idx].center

            if(abs(np.sqrt(np.sum(n*n)) - list[idx].radius)>0.000001):
                print('check', abs(np.sqrt(np.sum(n*n)) - list[idx].radius))

            n = n / np.sqrt(np.sum(n * n))

        elif list[idx].__class__.__name__ == 'Box':
            point_i = viewPoint + m*ray
            diff = LIMIT
            i = -1
            count = 0

            for normal in list[idx].normal:
                if abs(np.sum(normal[0:3] * point_i)-normal[3]) < diff:
                    diff = abs(np.sum(normal[0:3] * point_i)-normal[3])
                    i = count
                count = count + 1
            n = list[idx].normal[i][0:3]
            n = n / np.sqrt(np.sum(n * n))

        for i in light:
            l_i = v + i.position - viewPoint
            l_i = l_i / np.sqrt(np.sum(l_i * l_i))
            check = raytrace(list, -l_i, i.position)

            if check[1] == idx:
                if list[idx].shader.__class__.__name__ == 'Phong':
                    v_unit = v / np.sqrt(np.sum(v**2))
                    h = v_unit + l_i
                    h = h / np.sqrt(np.sum(h*h))
                    x = x + list[idx].shader.diffuse[0]*max(0,np.dot(n,l_i))*i.intensity[0] + list[idx].shader.specular[0] * i.intensity[0] * pow(max(0, np.dot(n, h)),list[idx].shader.exponent[0])
                    y = y + list[idx].shader.diffuse[1]*max(0,np.dot(n,l_i))*i.intensity[1] + list[idx].shader.specular[1] * i.intensity[1] * pow(max(0, np.dot(n, h)),list[idx].shader.exponent[0])
                    z = z + list[idx].shader.diffuse[2]*max(0,np.dot(n,l_i))*i.intensity[2] + list[idx].shader.specular[2] * i.intensity[2] * pow(max(0, np.dot(n, h)),list[idx].shader.exponent[0])

                elif list[idx].shader.__class__.__name__ == "Lambertian":
                    x = x + list[idx].shader.diffuse[0] * i.intensity[0] * max(0, np.dot(l_i, n))
                    y = y + list[idx].shader.diffuse[1] * i.intensity[1] * max(0, np.dot(l_i, n))
                    z = z + list[idx].shader.diffuse[2] * i.intensity[2] * max(0, np.dot(l_i, n))
        
        res = Color(x, y, z)
        res.gammaCorrect(2.2)
        return res.toUINT8()


def getNormal(x, y, z):
    dir = np.cross((y-x), (z-x))
    d = np.sum(dir*z)
    return np.array([dir[0], dir[1], dir[2], d])


def main():

    tree = ET.parse(sys.argv[1])
    root = tree.getroot()

    # set default values
    viewDir=np.array([0,0,-1]).astype(float)
    viewUp=np.array([0,1,0]).astype(float)
    viewProjNormal=-1*viewDir  # you can safely assume this. (no examples will use shifted perspective camera)
    viewWidth=1.0
    viewHeight=1.0
    projDistance=1.0
    intensity=np.array([1,1,1]).astype(float)  # how bright the light is.
    print(np.cross(viewDir, viewUp))

    light = []
    list = []

    imgSize=np.array(root.findtext('image').split()).astype(int)

    for c in root.findall('camera'):
        viewPoint=np.array(c.findtext('viewPoint').split()).astype(float)
        print('viewpoint', viewPoint)
        viewDir=np.array(c.findtext('viewDir').split()).astype(float)
        print('viewDir', viewDir)
        if(c.findtext('ProjNormal')):
            viewProjNormal=np.array(c.findtext('ProjNormal').split()).astype(float)
            print('ProjNormal', viewProjNormal)
        viewUp=np.array(c.findtext('viewUp').split()).astype(float)
        print('viewUp', viewUp)
        if(c.findtext('projDistance')):
            projDistance = np.array(c.findtext('projDistance').split()).astype(float)
            print('projDistance', projDistance)
        viewWidth=np.array(c.findtext('viewWidth').split()).astype(float)
        print('viewWidth', viewWidth)
        viewHeight=np.array(c.findtext('viewHeight').split()).astype(float)
        print('viewHeight', viewHeight)

    for c in root.findall('surface'):
        type_c = c.get('type')
        if type_c == 'Sphere':
            center_c = np.array(c.findtext('center').split()).astype(float)
            radius_c = np.array(c.findtext('radius')).astype(float)
            ref = ''
            for child in c:
                if child.tag == 'shader':
                    ref = child.get('ref')
            for d in root.findall('shader'):
                if d.get('name') == ref:
                    diffuse_d = np.array(d.findtext('diffuseColor').split()).astype(float)
                    type_d = d.get('type')
                    if type_d == 'Lambertian':
                        shader = Lambertian(diffuse_d)
                        list.append(Sphere(shader, center_c, radius_c))
                    elif type_d == 'Phong':
                        exponent_d = np.array(d.findtext('exponent').split()).astype(float)
                        specular_d = np.array(d.findtext('specularColor').split()).astype(float)
                        shader = Phong(diffuse_d, specular_d, exponent_d)
                        list.append(Sphere(shader, center_c, radius_c))
        elif type_c == 'Box':
            minPt_c = np.array(c.findtext('minPt').split()).astype(float)
            maxPt_c = np.array(c.findtext('maxPt').split()).astype(float)

            normals = []

            point_a = np.array([minPt_c[0], minPt_c[1], maxPt_c[2]])
            point_b = np.array([minPt_c[0], maxPt_c[1], minPt_c[2]])
            point_c = np.array([maxPt_c[0], minPt_c[1], minPt_c[2]])
            point_d = np.array([minPt_c[0], maxPt_c[1], maxPt_c[2]])
            point_e = np.array([maxPt_c[0], minPt_c[1], maxPt_c[2]])
            point_f = np.array([maxPt_c[0], maxPt_c[1], minPt_c[2]])

            normals.append(getNormal(point_a, point_c, point_e))
            normals.append(getNormal(point_b, point_c, point_f))
            normals.append(getNormal(point_a, point_b, point_d))
            normals.append(getNormal(point_a, point_e, point_d))
            normals.append(getNormal(point_e, point_c, point_f))
            normals.append(getNormal(point_d, point_f, point_b))

            ref = ''
            for child in c:
                if child.tag == 'shader':
                    ref = child.get('ref')
            for d in root.findall('shader'):
                if d.get('name') == ref:
                    diffuse_d = np.array(d.findtext('diffuseColor').split()).astype(float)
                    type_d = d.get('type')
                    if type_d == 'Lambertian':
                        shader = Lambertian(diffuse_d)
                        list.append(Box(shader, minPt_c, maxPt_c, normals))
                    elif type_d == 'Phong':
                        exponent_d = np.array(d.findtext('exponent').split()).astype(float)
                        specular_d = np.array(d.findtext('specularColor').split()).astype(float)
                        shader = Phong(diffuse_d, specular_d, exponent_d)
                        list.append(Box(shader, minPt_c, maxPt_c, normals))


    for c in root.findall('light'):
        position = np.array(c.findtext('position').split()).astype(float)
        intensity = np.array(c.findtext('intensity').split()).astype(float)
        light.append(Light(position, intensity))

    #code.interact(local=dict(globals(), **locals()))  

    # Create an empty image
    channels=3
    img = np.zeros((imgSize[1], imgSize[0], channels), dtype=np.uint8)
    img[:,:]=0

    w = viewDir
    u = np.cross(w, viewUp)
    v = np.cross(w, u)

    #unit vector
    unitw = w / np.linalg.norm(w)
    unitu = u / np.linalg.norm(u)
    unitv = v / np.linalg.norm(v)


    pixelx = viewWidth / imgSize[0]
    pixely = viewHeight / imgSize[1]


    origin = unitw * projDistance - unitu * pixelx * ((imgSize[0]/2) + 1/2) - unitv * pixely * ((imgSize[1]/2) + 1/2)

    for x in np.arange(imgSize[0]):
        for y in np.arange(imgSize[1]):
            ray = origin + unitu * x * pixelx + unitv * y * pixely
            tmp = raytrace(list, ray, viewPoint)
            img[y][x] = shade(tmp[0], ray, viewPoint, list, tmp[1], light)

    rawimg = Image.fromarray(img, 'RGB')
    #rawimg.save('out.png')
    rawimg.save(sys.argv[1]+'.png')
    
if __name__=="__main__":
    main()
