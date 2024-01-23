xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 243;
 -9.99965;0.00035;1.44322;,
 -8.83331;3.65937;1.47094;,
 -9.29273;3.84967;-0.11143;,
 -10.53789;0.00035;-0.11143;,
 -7.07065;7.07135;1.44322;,
 -7.45131;7.45201;-0.11143;,
 -8.83331;3.65938;-1.95515;,
 -9.99965;0.00035;-1.92743;,
 -7.07065;7.07135;-1.92743;,
 -3.65849;8.83235;1.47000;,
 -3.84910;9.29295;-0.11152;,
 0.00035;10.00035;1.44322;,
 -0.00010;10.53930;-0.11067;,
 -3.65867;8.83402;-1.95515;,
 0.00035;10.00035;-1.92743;,
 3.65809;8.83204;1.47139;,
 3.84926;9.29325;-0.11013;,
 7.07135;7.07135;1.44322;,
 7.45292;7.45292;-0.11004;,
 3.65938;8.83402;-1.95515;,
 7.07135;7.07135;-1.92743;,
 8.83204;3.65808;1.47139;,
 9.29325;3.84926;-0.11013;,
 10.00035;0.00035;1.44322;,
 10.53930;-0.00010;-0.11067;,
 8.83402;3.65938;-1.95515;,
 10.00035;0.00035;-1.92743;,
 8.83235;-3.65849;1.47000;,
 9.29295;-3.84910;-0.11152;,
 7.07135;-7.07065;1.44322;,
 7.45201;-7.45131;-0.11143;,
 8.83401;-3.65867;-1.95515;,
 7.07135;-7.07065;-1.92743;,
 3.65937;-8.83331;1.47094;,
 3.84967;-9.29273;-0.11143;,
 0.00035;-9.99965;1.44322;,
 0.00035;-10.53789;-0.11143;,
 3.65937;-8.83331;-1.95515;,
 0.00035;-9.99965;-1.92743;,
 -3.65867;-8.83331;1.47094;,
 -3.84897;-9.29273;-0.11143;,
 -7.07065;-7.07065;1.44322;,
 -7.45131;-7.45131;-0.11143;,
 -3.65867;-8.83331;-1.95515;,
 -7.07065;-7.07065;-1.92743;,
 -8.83331;-3.65867;1.47094;,
 -9.29273;-3.84897;-0.11143;,
 -9.99965;0.00035;1.44322;,
 -10.53789;0.00035;-0.11143;,
 -8.83331;-3.65867;-1.95515;,
 -9.99965;0.00035;-1.92743;,
 0.00035;-9.12766;1.45773;,
 -3.37272;-8.09722;1.45803;,
 -6.52003;-6.52003;1.42554;,
 -6.52003;-6.52003;-1.90743;,
 -3.37272;-8.09722;-1.93992;,
 0.00035;-9.12766;-1.93963;,
 -6.52003;-6.52003;1.42554;,
 -8.09722;-3.37272;1.45803;,
 -9.12766;0.00035;1.45773;,
 -9.12766;0.00035;-1.93963;,
 -8.09722;-3.37272;-1.93992;,
 -6.52003;-6.52003;-1.90743;,
 0.00010;9.12792;1.45772;,
 3.37297;8.09730;1.45433;,
 6.53049;6.58831;1.50943;,
 6.52073;6.52074;-1.90743;,
 3.37342;8.09793;-1.93992;,
 0.00035;9.12837;-1.93963;,
 -6.52003;6.52073;1.42554;,
 -3.37283;8.09759;1.45717;,
 0.00010;9.12792;1.45772;,
 -6.52003;6.52074;-1.90743;,
 -3.37272;8.09793;-1.93992;,
 0.00035;9.12837;-1.93963;,
 9.12792;0.00010;1.45772;,
 8.09759;-3.37283;1.45717;,
 6.52073;-6.52003;1.42554;,
 9.12836;0.00035;-1.93963;,
 8.09793;-3.37272;-1.93992;,
 6.52073;-6.52003;-1.90743;,
 -9.12766;0.00035;1.45773;,
 -8.09723;3.37342;1.45803;,
 -6.52003;6.52073;1.42554;,
 -9.12766;0.00035;-1.93963;,
 -8.09723;3.37342;-1.93992;,
 -6.52003;6.52074;-1.90743;,
 6.52073;-6.52003;1.42554;,
 3.37343;-8.09723;1.45803;,
 0.00035;-9.12766;1.45773;,
 6.52073;-6.52003;-1.90743;,
 3.37342;-8.09723;-1.93992;,
 0.00035;-9.12766;-1.93963;,
 6.53049;6.58831;1.50943;,
 8.09730;3.37297;1.45433;,
 9.12792;0.00010;1.45772;,
 6.52073;6.52074;-1.90743;,
 8.09793;3.37342;-1.93992;,
 9.12836;0.00035;-1.93963;,
 0.00035;-8.95488;1.04623;,
 -3.36115;-8.02065;1.07807;,
 -3.37272;-8.09722;1.45803;,
 0.00035;-9.12766;1.45773;,
 -6.46732;-6.46732;1.04623;,
 -6.52003;-6.52003;1.42554;,
 0.00035;-8.95488;1.04623;,
 -6.46732;-6.46732;1.04623;,
 0.00017;0.00017;0.78879;,
 -6.46732;-6.46732;1.04623;,
 -8.02065;-3.36115;1.07807;,
 -8.09722;-3.37272;1.45803;,
 -6.52003;-6.52003;1.42554;,
 -8.99965;0.00035;1.07180;,
 -9.12766;0.00035;1.45773;,
 -6.46732;-6.46732;1.04623;,
 -8.99965;0.00035;1.07180;,
 0.00017;0.00017;0.78879;,
 -0.00038;8.95559;1.04624;,
 3.36185;8.02185;1.06907;,
 3.37297;8.09730;1.45433;,
 0.00010;9.12792;1.45772;,
 6.50019;6.50019;1.05448;,
 6.53049;6.58831;1.50943;,
 -0.00038;8.95559;1.04624;,
 6.50019;6.50019;1.05448;,
 0.00017;0.00017;0.78879;,
 -6.46732;6.46802;1.04623;,
 -3.36165;8.02185;1.07757;,
 -3.37283;8.09759;1.45717;,
 -6.52003;6.52073;1.42554;,
 -0.00038;8.95559;1.04624;,
 0.00010;9.12792;1.45772;,
 -6.46732;6.46802;1.04623;,
 -0.00038;8.95559;1.04624;,
 0.00017;0.00017;0.78879;,
 9.00049;-0.00017;1.07165;,
 7.98085;-3.34465;1.05457;,
 8.09759;-3.37283;1.45717;,
 9.12792;0.00010;1.45772;,
 6.50035;-6.49965;1.07180;,
 6.52073;-6.52003;1.42554;,
 9.00049;-0.00017;1.07165;,
 6.50035;-6.49965;1.07180;,
 0.00017;0.00017;0.78879;,
 -8.99965;0.00035;1.07180;,
 -8.02065;3.36185;1.07807;,
 -8.09723;3.37342;1.45803;,
 -9.12766;0.00035;1.45773;,
 -6.46732;6.46802;1.04623;,
 -6.52003;6.52073;1.42554;,
 -8.99965;0.00035;1.07180;,
 -6.46732;6.46802;1.04623;,
 0.00017;0.00017;0.78879;,
 6.50035;-6.49965;1.07180;,
 3.36185;-8.02065;1.07807;,
 3.37343;-8.09723;1.45803;,
 6.52073;-6.52003;1.42554;,
 0.00035;-8.95488;1.04623;,
 0.00035;-9.12766;1.45773;,
 6.50035;-6.49965;1.07180;,
 0.00035;-8.95488;1.04623;,
 0.00017;0.00017;0.78879;,
 6.50019;6.50019;1.05448;,
 8.02185;3.36185;1.06907;,
 8.09730;3.37297;1.45433;,
 6.53049;6.58831;1.50943;,
 9.00049;-0.00017;1.07165;,
 9.12792;0.00010;1.45772;,
 6.50019;6.50019;1.05448;,
 9.00049;-0.00017;1.07165;,
 0.00017;0.00017;0.78879;,
 9.00036;0.00035;-1.11471;,
 8.02134;3.36186;-0.77086;,
 8.09793;3.37342;-1.93992;,
 9.12836;0.00035;-1.93963;,
 6.50036;6.50036;-1.11471;,
 6.52073;6.52074;-1.90743;,
 9.00036;0.00035;-1.11471;,
 6.50036;6.50036;-1.11471;,
 0.00035;0.00035;-1.11448;,
 6.50036;-6.49966;-1.11471;,
 8.02134;-3.36115;-0.77086;,
 8.09793;-3.37272;-1.93992;,
 6.52073;-6.52003;-1.90743;,
 9.00036;0.00035;-1.11471;,
 9.12836;0.00035;-1.93963;,
 6.50036;-6.49966;-1.11471;,
 9.00036;0.00035;-1.11471;,
 0.00035;0.00035;-1.11448;,
 0.00035;-8.99966;-1.11471;,
 3.38135;-8.06608;-0.94929;,
 3.37342;-8.09723;-1.93992;,
 0.00035;-9.12766;-1.93963;,
 6.50036;-6.49966;-1.11471;,
 6.52073;-6.52003;-1.90743;,
 0.00035;-8.99966;-1.11471;,
 6.50036;-6.49966;-1.11471;,
 0.00035;0.00035;-1.11448;,
 -6.49966;-6.49966;-1.11471;,
 -3.36115;-8.02064;-0.77086;,
 -3.37272;-8.09722;-1.93992;,
 -6.52003;-6.52003;-1.90743;,
 0.00035;-8.99966;-1.11471;,
 0.00035;-9.12766;-1.93963;,
 -6.49966;-6.49966;-1.11471;,
 0.00035;-8.99966;-1.11471;,
 0.00035;0.00035;-1.11448;,
 -8.99966;0.00035;-1.11471;,
 -8.02064;-3.36115;-0.77086;,
 -8.09722;-3.37272;-1.93992;,
 -9.12766;0.00035;-1.93963;,
 -6.49966;-6.49966;-1.11471;,
 -6.52003;-6.52003;-1.90743;,
 -8.99966;0.00035;-1.11471;,
 -6.49966;-6.49966;-1.11471;,
 0.00035;0.00035;-1.11448;,
 -6.49966;6.50036;-1.11471;,
 -8.02065;3.36185;-1.12093;,
 -8.09723;3.37342;-1.93992;,
 -6.52003;6.52074;-1.90743;,
 -8.99966;0.00035;-1.11471;,
 -9.12766;0.00035;-1.93963;,
 -6.49966;6.50036;-1.11471;,
 -8.99966;0.00035;-1.11471;,
 0.00035;0.00035;-1.11448;,
 0.00035;9.00036;-1.11471;,
 -3.36115;8.02134;-0.77086;,
 -3.37272;8.09793;-1.93992;,
 0.00035;9.12837;-1.93963;,
 -6.49966;6.50036;-1.11471;,
 -6.52003;6.52074;-1.90743;,
 0.00035;9.00036;-1.11471;,
 -6.49966;6.50036;-1.11471;,
 0.00035;0.00035;-1.11448;,
 6.50036;6.50036;-1.11471;,
 3.36186;8.02134;-0.77086;,
 3.37342;8.09793;-1.93992;,
 6.52073;6.52074;-1.90743;,
 0.00035;9.00036;-1.11471;,
 0.00035;9.12837;-1.93963;,
 6.50036;6.50036;-1.11471;,
 0.00035;9.00036;-1.11471;,
 0.00035;0.00035;-1.11448;;
 
 128;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;3,2,6,7;,
 4;2,5,8,6;,
 4;4,9,10,5;,
 4;9,11,12,10;,
 4;5,10,13,8;,
 4;10,12,14,13;,
 4;11,15,16,12;,
 4;15,17,18,16;,
 4;12,16,19,14;,
 4;16,18,20,19;,
 4;17,21,22,18;,
 4;21,23,24,22;,
 4;18,22,25,20;,
 4;22,24,26,25;,
 4;23,27,28,24;,
 4;27,29,30,28;,
 4;24,28,31,26;,
 4;28,30,32,31;,
 4;29,33,34,30;,
 4;33,35,36,34;,
 4;30,34,37,32;,
 4;34,36,38,37;,
 4;35,39,40,36;,
 4;39,41,42,40;,
 4;36,40,43,38;,
 4;40,42,44,43;,
 4;41,45,46,42;,
 4;45,47,48,46;,
 4;42,46,49,44;,
 4;46,48,50,49;,
 4;51,52,39,35;,
 4;52,53,41,39;,
 4;54,55,43,44;,
 4;55,56,38,43;,
 4;57,58,45,41;,
 4;58,59,47,45;,
 4;60,61,49,50;,
 4;61,62,44,49;,
 4;63,64,15,11;,
 4;64,65,17,15;,
 4;66,67,19,20;,
 4;67,68,14,19;,
 4;69,70,9,4;,
 4;70,71,11,9;,
 4;72,8,13,73;,
 4;73,13,14,74;,
 4;75,76,27,23;,
 4;76,77,29,27;,
 4;78,26,31,79;,
 4;79,31,32,80;,
 4;81,82,1,0;,
 4;82,83,4,1;,
 4;84,7,6,85;,
 4;85,6,8,86;,
 4;87,88,33,29;,
 4;88,89,35,33;,
 4;90,32,37,91;,
 4;91,37,38,92;,
 4;93,94,21,17;,
 4;94,95,23,21;,
 4;96,20,25,97;,
 4;97,25,26,98;,
 4;99,100,101,102;,
 4;100,103,104,101;,
 3;105,106,100;,
 3;105,107,106;,
 4;108,109,110,111;,
 4;109,112,113,110;,
 3;114,115,109;,
 3;114,116,115;,
 4;117,118,119,120;,
 4;118,121,122,119;,
 3;123,124,118;,
 3;123,125,124;,
 4;126,127,128,129;,
 4;127,130,131,128;,
 3;132,133,127;,
 3;132,134,133;,
 4;135,136,137,138;,
 4;136,139,140,137;,
 3;141,142,136;,
 3;141,143,142;,
 4;144,145,146,147;,
 4;145,148,149,146;,
 3;150,151,145;,
 3;150,152,151;,
 4;153,154,155,156;,
 4;154,157,158,155;,
 3;159,160,154;,
 3;159,161,160;,
 4;162,163,164,165;,
 4;163,166,167,164;,
 3;168,169,163;,
 3;168,170,169;,
 4;171,172,173,174;,
 4;172,175,176,173;,
 3;177,178,172;,
 3;177,179,178;,
 4;180,181,182,183;,
 4;181,184,185,182;,
 3;186,187,181;,
 3;186,188,187;,
 4;189,190,191,192;,
 4;190,193,194,191;,
 3;195,196,190;,
 3;195,197,196;,
 4;198,199,200,201;,
 4;199,202,203,200;,
 3;204,205,199;,
 3;204,206,205;,
 4;207,208,209,210;,
 4;208,211,212,209;,
 3;213,214,208;,
 3;213,215,214;,
 4;216,217,218,219;,
 4;217,220,221,218;,
 3;222,223,217;,
 3;222,224,223;,
 4;225,226,227,228;,
 4;226,229,230,227;,
 3;231,232,226;,
 3;231,233,232;,
 4;234,235,236,237;,
 4;235,238,239,236;,
 3;235,240,241;,
 3;241,240,242;;
 
 MeshMaterialList {
  1;
  128;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.743200;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  146;
  -0.947561;-0.000000;0.319574;,
  -0.879320;0.364232;0.306809;,
  -0.669887;0.670050;0.319820;,
  -0.999755;-0.000000;0.022114;,
  -0.923671;0.382603;0.021136;,
  -0.706880;0.706984;0.022250;,
  -0.960659;-0.000000;-0.277730;,
  -0.890509;0.368867;-0.266329;,
  -0.679276;0.679310;-0.277709;,
  -0.364234;0.879131;0.307347;,
  -0.000045;0.947283;0.320398;,
  -0.382655;0.923642;0.021435;,
  -0.000021;0.999746;0.022548;,
  -0.368906;0.890500;-0.266304;,
  0.000006;0.960664;-0.277714;,
  0.364057;0.879062;0.307754;,
  0.669745;0.669745;0.320755;,
  0.382552;0.923680;0.021656;,
  0.706925;0.706925;0.022700;,
  0.368839;0.890532;-0.266290;,
  0.679277;0.679277;-0.277786;,
  0.879062;0.364057;0.307753;,
  0.947283;-0.000045;0.320397;,
  0.923680;0.382552;0.021656;,
  0.999746;-0.000021;0.022548;,
  0.890532;0.368839;-0.266290;,
  0.960664;0.000006;-0.277715;,
  0.879131;-0.364234;0.307347;,
  0.670050;-0.669887;0.319820;,
  0.923643;-0.382655;0.021435;,
  0.706984;-0.706880;0.022250;,
  0.890500;-0.368906;-0.266305;,
  0.679310;-0.679276;-0.277710;,
  0.364232;-0.879320;0.306809;,
  0.000000;-0.947561;0.319574;,
  0.382603;-0.923671;0.021136;,
  0.000000;-0.999755;0.022114;,
  0.368867;-0.890509;-0.266330;,
  0.000000;-0.960659;-0.277730;,
  -0.364232;-0.879320;0.306809;,
  -0.670021;-0.670021;0.319600;,
  -0.382603;-0.923671;0.021136;,
  -0.706934;-0.706934;0.022115;,
  -0.368867;-0.890509;-0.266330;,
  -0.679284;-0.679284;-0.277753;,
  -0.879320;-0.364232;0.306809;,
  -0.923671;-0.382603;0.021136;,
  -0.890509;-0.368867;-0.266330;,
  -0.000000;-0.000493;1.000000;,
  0.002861;0.010897;0.999937;,
  0.012220;0.012220;0.999851;,
  0.014258;0.014258;-0.999797;,
  0.003975;0.013613;-0.999900;,
  -0.000000;0.002239;-0.999997;,
  0.010897;0.002861;0.999937;,
  -0.000493;-0.000000;1.000000;,
  0.002239;0.000000;-0.999997;,
  0.013613;0.003975;-0.999900;,
  -0.000299;-0.000886;1.000000;,
  0.004491;0.015610;0.999868;,
  0.022603;0.022930;0.999481;,
  -0.014256;-0.014256;-0.999797;,
  -0.003974;-0.013612;-0.999900;,
  0.000000;-0.002239;-0.999997;,
  0.012309;-0.012135;0.999851;,
  0.002827;-0.010823;0.999938;,
  0.014256;-0.014256;-0.999797;,
  0.003974;-0.013612;-0.999900;,
  -0.000886;-0.000299;1.000000;,
  -0.010823;0.002827;0.999938;,
  -0.012135;0.012309;0.999851;,
  -0.002239;0.000000;-0.999997;,
  -0.013613;0.003975;-0.999900;,
  -0.014258;0.014258;-0.999797;,
  0.010896;-0.002861;0.999937;,
  0.013612;-0.003974;-0.999899;,
  -0.002861;0.010896;0.999937;,
  -0.003975;0.013614;-0.999900;,
  0.015032;0.004240;0.999878;,
  -0.013612;-0.003974;-0.999899;,
  0.355311;0.903960;0.237928;,
  0.000001;0.954686;0.297616;,
  0.693307;0.693307;0.196596;,
  -0.000596;0.000333;1.000000;,
  0.907140;0.359010;0.219564;,
  0.965407;0.000000;0.260749;,
  -0.349066;-0.906076;0.239122;,
  0.000142;-0.955424;0.295237;,
  -0.693824;-0.694999;0.188637;,
  0.355405;-0.904273;0.236597;,
  0.693400;-0.693374;0.196032;,
  -0.904354;0.354906;0.237034;,
  -0.959614;0.000874;0.281320;,
  -0.698844;0.695959;0.165101;,
  0.907139;-0.359010;0.219567;,
  -0.355350;0.910462;0.211626;,
  -0.911433;-0.355727;0.206757;,
  -0.928061;-0.364204;-0.077839;,
  -0.993849;0.000000;-0.110745;,
  -0.706171;-0.706172;-0.051425;,
  0.000000;0.000000;-1.000000;,
  -0.928061;0.364204;-0.077837;,
  -0.708019;0.704988;-0.041229;,
  -0.364678;0.929273;-0.058831;,
  0.001957;0.994886;-0.100987;,
  0.364204;0.928061;-0.077837;,
  0.706171;0.706171;-0.051425;,
  0.928061;0.364204;-0.077837;,
  0.993013;-0.001420;-0.117998;,
  0.926994;-0.363772;-0.091392;,
  0.707014;-0.704781;-0.058436;,
  0.364204;-0.928061;-0.077841;,
  -0.000000;-0.993849;-0.110748;,
  -0.364204;-0.928061;-0.077839;,
  0.001353;0.050331;0.998732;,
  0.033613;0.036067;0.998784;,
  0.033986;0.088364;0.995508;,
  0.044673;0.000000;0.999002;,
  0.057900;0.018724;0.998147;,
  0.002564;-0.049886;0.998752;,
  -0.021414;-0.024595;0.999468;,
  -0.022087;-0.055124;0.998235;,
  0.033467;-0.035687;0.998803;,
  0.033407;-0.086849;0.995661;,
  -0.004678;-0.006768;0.999966;,
  -0.001725;0.017912;0.999838;,
  0.064146;-0.024653;0.997636;,
  0.057900;-0.018724;0.998147;,
  -0.024844;0.055379;0.998156;,
  -0.020019;-0.005059;0.999787;,
  0.395768;-0.000000;-0.918350;,
  0.274886;0.274886;-0.921344;,
  0.710463;0.273255;-0.648517;,
  0.233221;-0.182807;-0.955086;,
  0.710463;-0.273255;-0.648517;,
  -0.031454;-0.299946;-0.953438;,
  0.156176;-0.406058;-0.900403;,
  -0.274886;-0.274886;-0.921344;,
  -0.273255;-0.710463;-0.648517;,
  -0.185457;-0.075431;-0.979753;,
  -0.710463;-0.273255;-0.648517;,
  -0.068043;0.188722;-0.979670;,
  0.019828;-0.007626;-0.999774;,
  0.000000;0.395768;-0.918350;,
  -0.273255;0.710463;-0.648517;,
  0.273255;0.710463;-0.648517;;
  128;
  4;0,1,4,3;,
  4;1,2,5,4;,
  4;3,4,7,6;,
  4;4,5,8,7;,
  4;2,9,11,5;,
  4;9,10,12,11;,
  4;5,11,13,8;,
  4;11,12,14,13;,
  4;10,15,17,12;,
  4;15,16,18,17;,
  4;12,17,19,14;,
  4;17,18,20,19;,
  4;16,21,23,18;,
  4;21,22,24,23;,
  4;18,23,25,20;,
  4;23,24,26,25;,
  4;22,27,29,24;,
  4;27,28,30,29;,
  4;24,29,31,26;,
  4;29,30,32,31;,
  4;28,33,35,30;,
  4;33,34,36,35;,
  4;30,35,37,32;,
  4;35,36,38,37;,
  4;34,39,41,36;,
  4;39,40,42,41;,
  4;36,41,43,38;,
  4;41,42,44,43;,
  4;40,45,46,42;,
  4;45,0,3,46;,
  4;42,46,47,44;,
  4;46,3,6,47;,
  4;48,49,49,48;,
  4;49,50,50,49;,
  4;51,52,52,51;,
  4;52,53,53,52;,
  4;50,54,54,50;,
  4;54,55,55,54;,
  4;56,57,57,56;,
  4;57,51,51,57;,
  4;58,59,59,58;,
  4;59,60,60,59;,
  4;61,62,62,61;,
  4;62,63,63,62;,
  4;64,65,65,64;,
  4;65,58,58,65;,
  4;66,66,67,67;,
  4;67,67,63,63;,
  4;68,69,69,68;,
  4;69,70,70,69;,
  4;71,71,72,72;,
  4;72,72,73,73;,
  4;55,74,74,55;,
  4;74,64,64,74;,
  4;56,56,75,75;,
  4;75,75,66,66;,
  4;70,76,76,70;,
  4;76,48,48,76;,
  4;73,73,77,77;,
  4;77,77,53,53;,
  4;60,78,78,60;,
  4;78,68,68,78;,
  4;61,61,79,79;,
  4;79,79,71,71;,
  4;81,80,80,81;,
  4;80,82,82,80;,
  3;114,115,116;,
  3;114,83,115;,
  4;82,84,84,82;,
  4;84,85,85,84;,
  3;115,117,118;,
  3;115,83,117;,
  4;87,86,86,87;,
  4;86,88,88,86;,
  3;119,120,121;,
  3;119,83,120;,
  4;90,89,89,90;,
  4;89,87,87,89;,
  3;122,119,123;,
  3;122,83,119;,
  4;92,91,91,92;,
  4;91,93,93,91;,
  3;124,125,126;,
  3;124,83,125;,
  4;85,94,94,85;,
  4;94,90,90,94;,
  3;117,122,127;,
  3;117,83,122;,
  4;93,95,95,93;,
  4;95,81,81,95;,
  3;125,114,128;,
  3;125,83,114;,
  4;88,96,96,88;,
  4;96,92,92,96;,
  3;120,124,129;,
  3;120,83,124;,
  4;98,97,97,98;,
  4;97,99,99,97;,
  3;130,131,132;,
  3;130,100,131;,
  4;102,101,101,102;,
  4;101,98,98,101;,
  3;133,130,134;,
  3;133,100,130;,
  4;104,103,103,104;,
  4;103,102,102,103;,
  3;135,133,136;,
  3;135,100,133;,
  4;106,105,105,106;,
  4;105,104,104,105;,
  3;137,135,138;,
  3;137,100,135;,
  4;108,107,107,108;,
  4;107,106,106,107;,
  3;139,137,140;,
  3;139,100,137;,
  4;110,109,109,110;,
  4;109,108,108,109;,
  3;141,139,142;,
  3;141,100,139;,
  4;112,111,111,112;,
  4;111,110,110,111;,
  3;143,141,144;,
  3;143,100,141;,
  4;99,113,113,99;,
  4;113,112,112,113;,
  3;145,131,143;,
  3;143,131,100;;
 }
 MeshTextureCoords {
  243;
  0.000000;0.000000;,
  0.062500;0.000000;,
  0.062500;0.500000;,
  0.000000;0.500000;,
  0.125000;0.000000;,
  0.125000;0.500000;,
  0.062500;1.000000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.187500;0.000000;,
  0.187500;0.500000;,
  0.250000;0.000000;,
  0.250000;0.500000;,
  0.187500;1.000000;,
  0.250000;1.000000;,
  0.312500;0.000000;,
  0.312500;0.500000;,
  0.375000;0.000000;,
  0.375000;0.500000;,
  0.312500;1.000000;,
  0.375000;1.000000;,
  0.437500;0.000000;,
  0.437500;0.500000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.437500;1.000000;,
  0.500000;1.000000;,
  0.562500;0.000000;,
  0.562500;0.500000;,
  0.625000;0.000000;,
  0.625000;0.500000;,
  0.562500;1.000000;,
  0.625000;1.000000;,
  0.687500;0.000000;,
  0.687500;0.500000;,
  0.750000;0.000000;,
  0.750000;0.500000;,
  0.687500;1.000000;,
  0.750000;1.000000;,
  0.812500;0.000000;,
  0.812500;0.500000;,
  0.875000;0.000000;,
  0.875000;0.500000;,
  0.812500;1.000000;,
  0.875000;1.000000;,
  0.937500;0.000000;,
  0.937500;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.937500;1.000000;,
  1.000000;1.000000;,
  0.765550;0.000000;,
  0.812020;0.000000;,
  0.858490;0.000000;,
  0.854250;1.000000;,
  0.812160;1.000000;,
  0.770060;1.000000;,
  0.890420;0.000000;,
  0.934870;0.000000;,
  0.979310;0.000000;,
  0.977990;1.000000;,
  0.935200;1.000000;,
  0.892410;1.000000;,
  0.270880;0.000000;,
  0.310850;0.000000;,
  0.350810;0.000000;,
  0.354910;1.000000;,
  0.309900;1.000000;,
  0.264890;1.000000;,
  0.137790;0.000000;,
  0.187050;0.000000;,
  0.236310;0.000000;,
  0.139100;1.000000;,
  0.187080;1.000000;,
  0.235070;1.000000;,
  0.512790;0.000000;,
  0.563020;0.000000;,
  0.613260;0.000000;,
  0.511700;1.000000;,
  0.563060;1.000000;,
  0.614420;1.000000;,
  0.011250;0.000000;,
  0.065540;0.000000;,
  0.119830;0.000000;,
  0.014410;1.000000;,
  0.063520;1.000000;,
  0.112630;1.000000;,
  0.633580;0.000000;,
  0.687410;0.000000;,
  0.741240;0.000000;,
  0.633480;1.000000;,
  0.687410;1.000000;,
  0.741340;1.000000;,
  0.383450;0.000000;,
  0.437760;0.000000;,
  0.492060;0.000000;,
  0.382630;1.000000;,
  0.437770;1.000000;,
  0.492900;1.000000;,
  0.781090;0.000000;,
  0.811540;0.000000;,
  0.811540;0.000000;,
  0.781090;0.000000;,
  0.841980;0.000000;,
  0.841980;0.000000;,
  0.796800;0.000000;,
  0.827240;0.000000;,
  0.812500;0.000000;,
  0.905850;0.000000;,
  0.932240;0.000000;,
  0.932240;0.000000;,
  0.905850;0.000000;,
  0.958620;0.000000;,
  0.958620;0.000000;,
  0.921670;0.000000;,
  0.948060;0.000000;,
  0.937500;0.000000;,
  0.291770;0.000000;,
  0.309200;0.000000;,
  0.309200;0.000000;,
  0.291770;0.000000;,
  0.326630;0.000000;,
  0.326630;0.000000;,
  0.302130;0.000000;,
  0.319560;0.000000;,
  0.312500;0.000000;,
  0.150580;0.000000;,
  0.186600;0.000000;,
  0.186600;0.000000;,
  0.150580;0.000000;,
  0.222620;0.000000;,
  0.222620;0.000000;,
  0.169040;0.000000;,
  0.205060;0.000000;,
  0.187500;0.000000;,
  0.525580;0.000000;,
  0.563540;0.000000;,
  0.563540;0.000000;,
  0.525580;0.000000;,
  0.601510;0.000000;,
  0.601510;0.000000;,
  0.544040;0.000000;,
  0.582010;0.000000;,
  0.562500;0.000000;,
  0.022490;0.000000;,
  0.068580;0.000000;,
  0.068580;0.000000;,
  0.022490;0.000000;,
  0.114660;0.000000;,
  0.114660;0.000000;,
  0.042500;0.000000;,
  0.088580;0.000000;,
  0.062500;0.000000;,
  0.642160;0.000000;,
  0.687320;0.000000;,
  0.687320;0.000000;,
  0.642160;0.000000;,
  0.732480;0.000000;,
  0.732480;0.000000;,
  0.664830;0.000000;,
  0.709990;0.000000;,
  0.687500;0.000000;,
  0.391910;0.000000;,
  0.438010;0.000000;,
  0.438010;0.000000;,
  0.391910;0.000000;,
  0.484120;0.000000;,
  0.484120;0.000000;,
  0.414700;0.000000;,
  0.460810;0.000000;,
  0.437500;0.000000;,
  0.471610;1.000000;,
  0.433950;1.000000;,
  0.433950;1.000000;,
  0.471610;1.000000;,
  0.396290;1.000000;,
  0.396290;1.000000;,
  0.454560;1.000000;,
  0.416900;1.000000;,
  0.437500;1.000000;,
  0.596490;1.000000;,
  0.561040;1.000000;,
  0.561040;1.000000;,
  0.596490;1.000000;,
  0.525590;1.000000;,
  0.525590;1.000000;,
  0.579500;1.000000;,
  0.544040;1.000000;,
  0.562500;1.000000;,
  0.723080;1.000000;,
  0.686560;1.000000;,
  0.686560;1.000000;,
  0.723080;1.000000;,
  0.650030;1.000000;,
  0.650030;1.000000;,
  0.705290;1.000000;,
  0.668770;1.000000;,
  0.687500;1.000000;,
  0.833510;1.000000;,
  0.811820;1.000000;,
  0.811820;1.000000;,
  0.833510;1.000000;,
  0.790120;1.000000;,
  0.790120;1.000000;,
  0.823000;1.000000;,
  0.801310;1.000000;,
  0.812500;1.000000;,
  0.955970;1.000000;,
  0.932900;1.000000;,
  0.932900;1.000000;,
  0.955970;1.000000;,
  0.909830;1.000000;,
  0.909830;1.000000;,
  0.946740;1.000000;,
  0.923660;1.000000;,
  0.937500;1.000000;,
  0.087480;1.000000;,
  0.062390;1.000000;,
  0.062390;1.000000;,
  0.087480;1.000000;,
  0.037310;1.000000;,
  0.037310;1.000000;,
  0.074990;1.000000;,
  0.049910;1.000000;,
  0.062500;1.000000;,
  0.214580;1.000000;,
  0.186330;1.000000;,
  0.186330;1.000000;,
  0.214580;1.000000;,
  0.158090;1.000000;,
  0.158090;1.000000;,
  0.201040;1.000000;,
  0.172790;1.000000;,
  0.187500;1.000000;,
  0.334810;1.000000;,
  0.307370;1.000000;,
  0.307370;1.000000;,
  0.334810;1.000000;,
  0.279920;1.000000;,
  0.279920;1.000000;,
  0.323660;1.000000;,
  0.296210;1.000000;,
  0.312500;1.000000;;
 }
}