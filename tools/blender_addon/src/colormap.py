# ***************************************************************************
# This file is part of the GAMer software.
# Copyright (C) 2016-2018
# by Christopher Lee, Tom Bartol, John Moody, Rommie Amaro, J. Andrew McCammon,
#    and Michael Holst

# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
# ***************************************************************************


import numpy as np

def getColor(values, minV=-1000, maxV=1000):
    print("Minimum value: %s; Maximum value: %s"%(np.amin(values),np.amax(values)))
    data = values
    # Truncate at min and max
    data[data < minV] = minV
    data[data > maxV] = maxV
    amin = np.amin(data)

    print("PostTruncation--Minimum value: %s; Maximum value: %s"%(np.amin(data),np.amax(data)))
    # Set lower bound to 0
    if(amin < 0):
        data = data + np.absolute(amin)
    else:
        data = data - amin
    amax = np.amax(data)

    # normalize to be between 0 and 1
    data = data/amax
    # print(data)
    chunk = 1/(len(BuRd)-2)
    remainder = np.mod(data, chunk)
    idx = np.floor_divide(data, chunk).astype(int)
    colors = BuRd[idx]*(1-remainder)[:,None] + BuRd[idx+1]*(remainder)[:,None]
    return colors


viridis = np.array([[0.26666667, 0.00392157, 0.32941176],
       [0.26666667, 0.00784314, 0.3372549 ],
       [0.27058824, 0.01568627, 0.34117647],
       [0.27058824, 0.01960784, 0.34901961],
       [0.2745098 , 0.02745098, 0.35294118],
       [0.2745098 , 0.03137255, 0.36078431],
       [0.2745098 , 0.03921569, 0.36470588],
       [0.2745098 , 0.04313725, 0.36862745],
       [0.27843137, 0.05098039, 0.37647059],
       [0.27843137, 0.05490196, 0.38039216],
       [0.27843137, 0.0627451 , 0.38823529],
       [0.27843137, 0.06666667, 0.39215686],
       [0.27843137, 0.0745098 , 0.39607843],
       [0.28235294, 0.07843137, 0.40392157],
       [0.28235294, 0.08627451, 0.40784314],
       [0.28235294, 0.09019608, 0.41176471],
       [0.28235294, 0.09411765, 0.41568627],
       [0.28235294, 0.10196078, 0.42352941],
       [0.28235294, 0.10588235, 0.42745098],
       [0.28235294, 0.10980392, 0.43137255],
       [0.28235294, 0.11372549, 0.43529412],
       [0.28235294, 0.12156863, 0.43921569],
       [0.28235294, 0.1254902 , 0.44313725],
       [0.28235294, 0.12941176, 0.45098039],
       [0.28235294, 0.1372549 , 0.45490196],
       [0.28235294, 0.14117647, 0.45882353],
       [0.28235294, 0.14509804, 0.4627451 ],
       [0.28235294, 0.14901961, 0.46666667],
       [0.28235294, 0.15686275, 0.47058824],
       [0.28235294, 0.16078431, 0.4745098 ],
       [0.27843137, 0.16470588, 0.47843137],
       [0.27843137, 0.17254902, 0.47843137],
       [0.27843137, 0.17647059, 0.48235294],
       [0.27843137, 0.18039216, 0.48627451],
       [0.27843137, 0.18431373, 0.49019608],
       [0.2745098 , 0.18823529, 0.49411765],
       [0.2745098 , 0.19607843, 0.49411765],
       [0.2745098 , 0.2       , 0.49803922],
       [0.2745098 , 0.20392157, 0.50196078],
       [0.27058824, 0.20784314, 0.50588235],
       [0.27058824, 0.21568627, 0.50588235],
       [0.27058824, 0.21960784, 0.50980392],
       [0.26666667, 0.22352941, 0.51372549],
       [0.26666667, 0.22745098, 0.51372549],
       [0.26666667, 0.23137255, 0.51764706],
       [0.2627451 , 0.23921569, 0.51764706],
       [0.2627451 , 0.24313725, 0.52156863],
       [0.25882353, 0.24705882, 0.52156863],
       [0.25882353, 0.25098039, 0.5254902 ],
       [0.25882353, 0.25490196, 0.5254902 ],
       [0.25490196, 0.25882353, 0.52941176],
       [0.25490196, 0.26666667, 0.52941176],
       [0.25098039, 0.27058824, 0.53333333],
       [0.25098039, 0.2745098 , 0.53333333],
       [0.24705882, 0.27843137, 0.53333333],
       [0.24705882, 0.28235294, 0.5372549 ],
       [0.24313725, 0.28627451, 0.5372549 ],
       [0.24313725, 0.29019608, 0.5372549 ],
       [0.24313725, 0.29803922, 0.54117647],
       [0.23921569, 0.30196078, 0.54117647],
       [0.23921569, 0.30588235, 0.54117647],
       [0.23529412, 0.30980392, 0.54117647],
       [0.23529412, 0.31372549, 0.54509804],
       [0.23137255, 0.31764706, 0.54509804],
       [0.23137255, 0.32156863, 0.54509804],
       [0.22745098, 0.3254902 , 0.54509804],
       [0.22745098, 0.32941176, 0.54901961],
       [0.22352941, 0.33333333, 0.54901961],
       [0.22352941, 0.3372549 , 0.54901961],
       [0.21960784, 0.34509804, 0.54901961],
       [0.21960784, 0.34901961, 0.54901961],
       [0.21568627, 0.35294118, 0.54901961],
       [0.21568627, 0.35686275, 0.55294118],
       [0.21176471, 0.36078431, 0.55294118],
       [0.21176471, 0.36470588, 0.55294118],
       [0.20784314, 0.36862745, 0.55294118],
       [0.20784314, 0.37254902, 0.55294118],
       [0.20392157, 0.37647059, 0.55294118],
       [0.20392157, 0.38039216, 0.55294118],
       [0.2       , 0.38431373, 0.55294118],
       [0.2       , 0.38823529, 0.55294118],
       [0.19607843, 0.39215686, 0.55686275],
       [0.19607843, 0.39607843, 0.55686275],
       [0.19215686, 0.4       , 0.55686275],
       [0.19215686, 0.40392157, 0.55686275],
       [0.19215686, 0.40784314, 0.55686275],
       [0.18823529, 0.41176471, 0.55686275],
       [0.18823529, 0.41568627, 0.55686275],
       [0.18431373, 0.41960784, 0.55686275],
       [0.18431373, 0.42352941, 0.55686275],
       [0.18039216, 0.42745098, 0.55686275],
       [0.18039216, 0.43137255, 0.55686275],
       [0.18039216, 0.43529412, 0.55686275],
       [0.17647059, 0.43921569, 0.55686275],
       [0.17647059, 0.44313725, 0.55686275],
       [0.17254902, 0.44313725, 0.55686275],
       [0.17254902, 0.44705882, 0.55686275],
       [0.17254902, 0.45098039, 0.55686275],
       [0.16862745, 0.45490196, 0.55686275],
       [0.16862745, 0.45882353, 0.55686275],
       [0.16470588, 0.4627451 , 0.55686275],
       [0.16470588, 0.46666667, 0.55686275],
       [0.16470588, 0.47058824, 0.55686275],
       [0.16078431, 0.4745098 , 0.55686275],
       [0.16078431, 0.47843137, 0.55686275],
       [0.16078431, 0.48235294, 0.55686275],
       [0.15686275, 0.48627451, 0.55686275],
       [0.15686275, 0.49019608, 0.55686275],
       [0.15294118, 0.49411765, 0.55686275],
       [0.15294118, 0.49803922, 0.55686275],
       [0.15294118, 0.50196078, 0.55686275],
       [0.14901961, 0.50588235, 0.55686275],
       [0.14901961, 0.50980392, 0.55686275],
       [0.14901961, 0.50980392, 0.55686275],
       [0.14509804, 0.51372549, 0.55686275],
       [0.14509804, 0.51764706, 0.55686275],
       [0.14509804, 0.52156863, 0.55686275],
       [0.14117647, 0.5254902 , 0.55686275],
       [0.14117647, 0.52941176, 0.55686275],
       [0.1372549 , 0.53333333, 0.55686275],
       [0.1372549 , 0.5372549 , 0.55686275],
       [0.1372549 , 0.54117647, 0.55294118],
       [0.13333333, 0.54509804, 0.55294118],
       [0.13333333, 0.54901961, 0.55294118],
       [0.13333333, 0.55294118, 0.55294118],
       [0.12941176, 0.55686275, 0.55294118],
       [0.12941176, 0.56078431, 0.55294118],
       [0.12941176, 0.56470588, 0.55294118],
       [0.12941176, 0.56862745, 0.54901961],
       [0.1254902 , 0.57254902, 0.54901961],
       [0.1254902 , 0.57254902, 0.54901961],
       [0.1254902 , 0.57647059, 0.54901961],
       [0.12156863, 0.58039216, 0.54901961],
       [0.12156863, 0.58431373, 0.54509804],
       [0.12156863, 0.58823529, 0.54509804],
       [0.12156863, 0.59215686, 0.54509804],
       [0.12156863, 0.59607843, 0.54509804],
       [0.12156863, 0.6       , 0.54117647],
       [0.12156863, 0.60392157, 0.54117647],
       [0.11764706, 0.60784314, 0.54117647],
       [0.11764706, 0.61176471, 0.5372549 ],
       [0.11764706, 0.61568627, 0.5372549 ],
       [0.12156863, 0.61960784, 0.5372549 ],
       [0.12156863, 0.62352941, 0.53333333],
       [0.12156863, 0.62745098, 0.53333333],
       [0.12156863, 0.63137255, 0.53333333],
       [0.12156863, 0.63137255, 0.52941176],
       [0.12156863, 0.63529412, 0.52941176],
       [0.1254902 , 0.63921569, 0.5254902 ],
       [0.1254902 , 0.64313725, 0.5254902 ],
       [0.12941176, 0.64705882, 0.52156863],
       [0.12941176, 0.65098039, 0.52156863],
       [0.13333333, 0.65490196, 0.52156863],
       [0.13333333, 0.65882353, 0.51764706],
       [0.1372549 , 0.6627451 , 0.51372549],
       [0.14117647, 0.66666667, 0.51372549],
       [0.14509804, 0.67058824, 0.50980392],
       [0.14509804, 0.6745098 , 0.50980392],
       [0.14901961, 0.67843137, 0.50588235],
       [0.15294118, 0.67843137, 0.50588235],
       [0.15686275, 0.68235294, 0.50196078],
       [0.16078431, 0.68627451, 0.49803922],
       [0.16470588, 0.69019608, 0.49803922],
       [0.17254902, 0.69411765, 0.49411765],
       [0.17647059, 0.69803922, 0.49019608],
       [0.18039216, 0.70196078, 0.48627451],
       [0.18431373, 0.70588235, 0.48627451],
       [0.19215686, 0.70980392, 0.48235294],
       [0.19607843, 0.71372549, 0.47843137],
       [0.20392157, 0.71372549, 0.4745098 ],
       [0.20784314, 0.71764706, 0.4745098 ],
       [0.21568627, 0.72156863, 0.47058824],
       [0.21960784, 0.7254902 , 0.46666667],
       [0.22745098, 0.72941176, 0.4627451 ],
       [0.23137255, 0.73333333, 0.45882353],
       [0.23921569, 0.7372549 , 0.45490196],
       [0.24705882, 0.7372549 , 0.45098039],
       [0.25098039, 0.74117647, 0.44705882],
       [0.25882353, 0.74509804, 0.44313725],
       [0.26666667, 0.74901961, 0.43921569],
       [0.2745098 , 0.75294118, 0.43529412],
       [0.28235294, 0.75686275, 0.43137255],
       [0.29019608, 0.75686275, 0.42745098],
       [0.29803922, 0.76078431, 0.42352941],
       [0.30588235, 0.76470588, 0.41960784],
       [0.31372549, 0.76862745, 0.41568627],
       [0.32156863, 0.77254902, 0.41176471],
       [0.32941176, 0.77254902, 0.40784314],
       [0.3372549 , 0.77647059, 0.40392157],
       [0.34509804, 0.78039216, 0.39607843],
       [0.35294118, 0.78431373, 0.39215686],
       [0.36078431, 0.78431373, 0.38823529],
       [0.36862745, 0.78823529, 0.38431373],
       [0.37647059, 0.79215686, 0.37647059],
       [0.38823529, 0.79607843, 0.37254902],
       [0.39607843, 0.79607843, 0.36862745],
       [0.40392157, 0.8       , 0.36078431],
       [0.41176471, 0.80392157, 0.35686275],
       [0.42352941, 0.80392157, 0.35294118],
       [0.43137255, 0.80784314, 0.34509804],
       [0.43921569, 0.81176471, 0.34117647],
       [0.45098039, 0.81568627, 0.3372549 ],
       [0.45882353, 0.81568627, 0.32941176],
       [0.46666667, 0.81960784, 0.3254902 ],
       [0.47843137, 0.81960784, 0.31764706],
       [0.48627451, 0.82352941, 0.31372549],
       [0.49803922, 0.82745098, 0.30588235],
       [0.50588235, 0.82745098, 0.30196078],
       [0.51764706, 0.83137255, 0.29411765],
       [0.5254902 , 0.83529412, 0.28627451],
       [0.5372549 , 0.83529412, 0.28235294],
       [0.54509804, 0.83921569, 0.2745098 ],
       [0.55686275, 0.83921569, 0.27058824],
       [0.56470588, 0.84313725, 0.2627451 ],
       [0.57647059, 0.84313725, 0.25490196],
       [0.58431373, 0.84705882, 0.25098039],
       [0.59607843, 0.84705882, 0.24313725],
       [0.60784314, 0.85098039, 0.23529412],
       [0.61568627, 0.85098039, 0.23137255],
       [0.62745098, 0.85490196, 0.22352941],
       [0.63529412, 0.85490196, 0.21568627],
       [0.64705882, 0.85882353, 0.21176471],
       [0.65882353, 0.85882353, 0.20392157],
       [0.66666667, 0.8627451 , 0.19607843],
       [0.67843137, 0.8627451 , 0.18823529],
       [0.69019608, 0.86666667, 0.18431373],
       [0.69803922, 0.86666667, 0.17647059],
       [0.70980392, 0.87058824, 0.16862745],
       [0.72156863, 0.87058824, 0.16078431],
       [0.72941176, 0.87058824, 0.15686275],
       [0.74117647, 0.8745098 , 0.14901961],
       [0.75294118, 0.8745098 , 0.14509804],
       [0.76078431, 0.8745098 , 0.1372549 ],
       [0.77254902, 0.87843137, 0.12941176],
       [0.78431373, 0.87843137, 0.1254902 ],
       [0.79215686, 0.88235294, 0.12156863],
       [0.80392157, 0.88235294, 0.11372549],
       [0.81568627, 0.88235294, 0.10980392],
       [0.82352941, 0.88627451, 0.10588235],
       [0.83529412, 0.88627451, 0.10196078],
       [0.84705882, 0.88627451, 0.09803922],
       [0.85490196, 0.89019608, 0.09803922],
       [0.86666667, 0.89019608, 0.09411765],
       [0.8745098 , 0.89019608, 0.09411765],
       [0.88627451, 0.89411765, 0.09411765],
       [0.89803922, 0.89411765, 0.09803922],
       [0.90588235, 0.89411765, 0.09803922],
       [0.91764706, 0.89803922, 0.10196078],
       [0.9254902 , 0.89803922, 0.10588235],
       [0.9372549 , 0.89803922, 0.10980392],
       [0.94509804, 0.89803922, 0.11372549],
       [0.95686275, 0.90196078, 0.11764706],
       [0.96470588, 0.90196078, 0.1254902 ],
       [0.97254902, 0.90196078, 0.12941176],
       [0.98431373, 0.90588235, 0.1372549 ],
       [0.99215686, 0.90588235, 0.14509804]])

# From matplotlib
_VIRIDIS_DATA = [[68, 1, 84],
                 [68, 2, 86],
                 [69, 4, 87],
                 [69, 5, 89],
                 [70, 7, 90],
                 [70, 8, 92],
                 [70, 10, 93],
                 [70, 11, 94],
                 [71, 13, 96],
                 [71, 14, 97],
                 [71, 16, 99],
                 [71, 17, 100],
                 [71, 19, 101],
                 [72, 20, 103],
                 [72, 22, 104],
                 [72, 23, 105],
                 [72, 24, 106],
                 [72, 26, 108],
                 [72, 27, 109],
                 [72, 28, 110],
                 [72, 29, 111],
                 [72, 31, 112],
                 [72, 32, 113],
                 [72, 33, 115],
                 [72, 35, 116],
                 [72, 36, 117],
                 [72, 37, 118],
                 [72, 38, 119],
                 [72, 40, 120],
                 [72, 41, 121],
                 [71, 42, 122],
                 [71, 44, 122],
                 [71, 45, 123],
                 [71, 46, 124],
                 [71, 47, 125],
                 [70, 48, 126],
                 [70, 50, 126],
                 [70, 51, 127],
                 [70, 52, 128],
                 [69, 53, 129],
                 [69, 55, 129],
                 [69, 56, 130],
                 [68, 57, 131],
                 [68, 58, 131],
                 [68, 59, 132],
                 [67, 61, 132],
                 [67, 62, 133],
                 [66, 63, 133],
                 [66, 64, 134],
                 [66, 65, 134],
                 [65, 66, 135],
                 [65, 68, 135],
                 [64, 69, 136],
                 [64, 70, 136],
                 [63, 71, 136],
                 [63, 72, 137],
                 [62, 73, 137],
                 [62, 74, 137],
                 [62, 76, 138],
                 [61, 77, 138],
                 [61, 78, 138],
                 [60, 79, 138],
                 [60, 80, 139],
                 [59, 81, 139],
                 [59, 82, 139],
                 [58, 83, 139],
                 [58, 84, 140],
                 [57, 85, 140],
                 [57, 86, 140],
                 [56, 88, 140],
                 [56, 89, 140],
                 [55, 90, 140],
                 [55, 91, 141],
                 [54, 92, 141],
                 [54, 93, 141],
                 [53, 94, 141],
                 [53, 95, 141],
                 [52, 96, 141],
                 [52, 97, 141],
                 [51, 98, 141],
                 [51, 99, 141],
                 [50, 100, 142],
                 [50, 101, 142],
                 [49, 102, 142],
                 [49, 103, 142],
                 [49, 104, 142],
                 [48, 105, 142],
                 [48, 106, 142],
                 [47, 107, 142],
                 [47, 108, 142],
                 [46, 109, 142],
                 [46, 110, 142],
                 [46, 111, 142],
                 [45, 112, 142],
                 [45, 113, 142],
                 [44, 113, 142],
                 [44, 114, 142],
                 [44, 115, 142],
                 [43, 116, 142],
                 [43, 117, 142],
                 [42, 118, 142],
                 [42, 119, 142],
                 [42, 120, 142],
                 [41, 121, 142],
                 [41, 122, 142],
                 [41, 123, 142],
                 [40, 124, 142],
                 [40, 125, 142],
                 [39, 126, 142],
                 [39, 127, 142],
                 [39, 128, 142],
                 [38, 129, 142],
                 [38, 130, 142],
                 [38, 130, 142],
                 [37, 131, 142],
                 [37, 132, 142],
                 [37, 133, 142],
                 [36, 134, 142],
                 [36, 135, 142],
                 [35, 136, 142],
                 [35, 137, 142],
                 [35, 138, 141],
                 [34, 139, 141],
                 [34, 140, 141],
                 [34, 141, 141],
                 [33, 142, 141],
                 [33, 143, 141],
                 [33, 144, 141],
                 [33, 145, 140],
                 [32, 146, 140],
                 [32, 146, 140],
                 [32, 147, 140],
                 [31, 148, 140],
                 [31, 149, 139],
                 [31, 150, 139],
                 [31, 151, 139],
                 [31, 152, 139],
                 [31, 153, 138],
                 [31, 154, 138],
                 [30, 155, 138],
                 [30, 156, 137],
                 [30, 157, 137],
                 [31, 158, 137],
                 [31, 159, 136],
                 [31, 160, 136],
                 [31, 161, 136],
                 [31, 161, 135],
                 [31, 162, 135],
                 [32, 163, 134],
                 [32, 164, 134],
                 [33, 165, 133],
                 [33, 166, 133],
                 [34, 167, 133],
                 [34, 168, 132],
                 [35, 169, 131],
                 [36, 170, 131],
                 [37, 171, 130],
                 [37, 172, 130],
                 [38, 173, 129],
                 [39, 173, 129],
                 [40, 174, 128],
                 [41, 175, 127],
                 [42, 176, 127],
                 [44, 177, 126],
                 [45, 178, 125],
                 [46, 179, 124],
                 [47, 180, 124],
                 [49, 181, 123],
                 [50, 182, 122],
                 [52, 182, 121],
                 [53, 183, 121],
                 [55, 184, 120],
                 [56, 185, 119],
                 [58, 186, 118],
                 [59, 187, 117],
                 [61, 188, 116],
                 [63, 188, 115],
                 [64, 189, 114],
                 [66, 190, 113],
                 [68, 191, 112],
                 [70, 192, 111],
                 [72, 193, 110],
                 [74, 193, 109],
                 [76, 194, 108],
                 [78, 195, 107],
                 [80, 196, 106],
                 [82, 197, 105],
                 [84, 197, 104],
                 [86, 198, 103],
                 [88, 199, 101],
                 [90, 200, 100],
                 [92, 200, 99],
                 [94, 201, 98],
                 [96, 202, 96],
                 [99, 203, 95],
                 [101, 203, 94],
                 [103, 204, 92],
                 [105, 205, 91],
                 [108, 205, 90],
                 [110, 206, 88],
                 [112, 207, 87],
                 [115, 208, 86],
                 [117, 208, 84],
                 [119, 209, 83],
                 [122, 209, 81],
                 [124, 210, 80],
                 [127, 211, 78],
                 [129, 211, 77],
                 [132, 212, 75],
                 [134, 213, 73],
                 [137, 213, 72],
                 [139, 214, 70],
                 [142, 214, 69],
                 [144, 215, 67],
                 [147, 215, 65],
                 [149, 216, 64],
                 [152, 216, 62],
                 [155, 217, 60],
                 [157, 217, 59],
                 [160, 218, 57],
                 [162, 218, 55],
                 [165, 219, 54],
                 [168, 219, 52],
                 [170, 220, 50],
                 [173, 220, 48],
                 [176, 221, 47],
                 [178, 221, 45],
                 [181, 222, 43],
                 [184, 222, 41],
                 [186, 222, 40],
                 [189, 223, 38],
                 [192, 223, 37],
                 [194, 223, 35],
                 [197, 224, 33],
                 [200, 224, 32],
                 [202, 225, 31],
                 [205, 225, 29],
                 [208, 225, 28],
                 [210, 226, 27],
                 [213, 226, 26],
                 [216, 226, 25],
                 [218, 227, 25],
                 [221, 227, 24],
                 [223, 227, 24],
                 [226, 228, 24],
                 [229, 228, 25],
                 [231, 228, 25],
                 [234, 229, 26],
                 [236, 229, 27],
                 [239, 229, 28],
                 [241, 229, 29],
                 [244, 230, 30],
                 [246, 230, 32],
                 [248, 230, 33],
                 [251, 231, 35],
                 [253, 231, 37]]




# import numpy as np
# from palettable.colorbrewer.diverging import RdBu_7_r
# import matplotlib as mpl
# np.array(mpl.colors.ListedColormap(RdBu_7_r.mpl_colors,N=256))
BuRd = np.array([[0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745],
       [0.99215686, 0.85882353, 0.78039216],
       [0.9372549 , 0.54117647, 0.38431373],
       [0.69803922, 0.09411765, 0.16862745],
       [0.12941176, 0.4       , 0.6745098 ],
       [0.40392157, 0.6627451 , 0.81176471],
       [0.81960784, 0.89803922, 0.94117647],
       [0.96862745, 0.96862745, 0.96862745]])