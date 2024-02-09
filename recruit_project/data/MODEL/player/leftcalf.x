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
 56;
 0.78483;-14.47104;-0.65167;,
 0.21327;-14.47104;-0.94130;,
 0.37990;-13.83969;-1.43458;,
 1.11661;-13.83969;-1.05428;,
 -0.94016;-14.47104;-0.94130;,
 -1.10679;-13.83969;-1.43458;,
 -1.51171;-14.47104;-0.65167;,
 -1.84350;-13.83969;-1.05428;,
 0.37990;-0.44094;-1.43458;,
 1.11661;-0.44094;-1.05428;,
 -1.10679;-0.44094;-1.43458;,
 -1.84350;-0.44094;-1.05428;,
 0.27777;0.21431;-1.19289;,
 0.91325;0.21431;-0.85702;,
 -1.00466;0.21431;-1.19289;,
 -1.64013;0.21431;-0.85702;,
 0.37990;0.80270;-0.05532;,
 1.56019;0.21431;0.02524;,
 -1.10679;0.80270;-0.05532;,
 -2.28708;0.21431;0.02524;,
 0.37990;0.80270;1.32393;,
 1.56019;0.21431;1.24337;,
 -1.10679;0.80270;1.32393;,
 -2.28708;0.21431;1.24337;,
 0.27777;0.21431;2.46150;,
 0.91325;0.21431;2.12562;,
 -1.00466;0.21431;2.46150;,
 -1.64013;0.21431;2.12562;,
 0.37990;-0.44094;2.70319;,
 1.11661;-0.44094;2.32289;,
 -1.10679;-0.44094;2.70319;,
 -1.84350;-0.44094;2.32289;,
 0.37990;-13.83969;2.70319;,
 1.11661;-13.83969;2.32289;,
 -1.10679;-13.83969;2.70319;,
 -1.84350;-13.83969;2.32289;,
 0.21327;-14.47104;2.20991;,
 0.78483;-14.47104;1.92028;,
 -0.94016;-14.47104;2.20991;,
 -1.51171;-14.47104;1.92028;,
 0.37990;-15.08333;1.32393;,
 1.36670;-14.47104;1.15950;,
 -1.10679;-15.08333;1.32393;,
 -2.09357;-14.47104;1.15950;,
 0.37990;-15.08333;-0.05533;,
 1.36670;-14.47104;0.10910;,
 -1.10679;-15.08333;-0.05533;,
 -2.09357;-14.47104;0.10910;,
 -2.59348;-13.83969;1.32393;,
 -2.59348;-13.83969;-0.05533;,
 -2.59348;-0.44094;1.32393;,
 -2.59348;-0.44094;-0.05533;,
 1.86661;-13.83969;1.32393;,
 1.86661;-13.83969;-0.05533;,
 1.86661;-0.44094;1.32393;,
 1.86661;-0.44094;-0.05533;;
 
 54;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;3,2,8,9;,
 4;2,5,10,8;,
 4;5,7,11,10;,
 4;9,8,12,13;,
 4;8,10,14,12;,
 4;10,11,15,14;,
 4;13,12,16,17;,
 4;12,14,18,16;,
 4;14,15,19,18;,
 4;17,16,20,21;,
 4;16,18,22,20;,
 4;18,19,23,22;,
 4;21,20,24,25;,
 4;20,22,26,24;,
 4;22,23,27,26;,
 4;25,24,28,29;,
 4;24,26,30,28;,
 4;26,27,31,30;,
 4;29,28,32,33;,
 4;28,30,34,32;,
 4;30,31,35,34;,
 4;33,32,36,37;,
 4;32,34,38,36;,
 4;34,35,39,38;,
 4;37,36,40,41;,
 4;36,38,42,40;,
 4;38,39,43,42;,
 4;41,40,44,45;,
 4;40,42,46,44;,
 4;42,43,47,46;,
 4;45,44,1,0;,
 4;44,46,4,1;,
 4;46,47,6,4;,
 4;43,39,35,48;,
 4;47,43,48,49;,
 4;6,47,49,7;,
 4;48,35,31,50;,
 4;49,48,50,51;,
 4;7,49,51,11;,
 4;50,31,27,23;,
 4;51,50,23,19;,
 4;11,51,19,15;,
 4;37,41,52,33;,
 4;41,45,53,52;,
 4;45,0,3,53;,
 4;33,52,54,29;,
 4;52,53,55,54;,
 4;53,3,9,55;,
 4;29,54,21,25;,
 4;54,55,17,21;,
 4;55,9,13,17;;
 
 MeshMaterialList {
  5;
  54;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\UV\\player_recruit_UV.png";
   }
  }
  Material {
   0.109600;0.200800;0.310400;1.000000;;
   39.000000;
   0.400000;0.400000;0.400000;;
   0.086310;0.158130;0.244440;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   39.000000;
   0.400000;0.400000;0.400000;;
   0.630000;0.630000;0.630000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   39.000000;
   0.400000;0.400000;0.400000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  56;
  0.470741;-0.680596;-0.561419;,
  0.198281;-0.728598;-0.655614;,
  -0.198282;-0.728598;-0.655614;,
  -0.470742;-0.680596;-0.561419;,
  0.606025;-0.339920;-0.719158;,
  0.220616;-0.338446;-0.914758;,
  -0.220616;-0.338446;-0.914758;,
  -0.606025;-0.339920;-0.719158;,
  0.633456;0.202881;-0.746708;,
  0.232226;0.188556;-0.954211;,
  -0.232226;0.188556;-0.954211;,
  -0.633456;0.202881;-0.746708;,
  0.532529;0.574116;-0.621936;,
  0.210715;0.669904;-0.711919;,
  -0.210715;0.669904;-0.711919;,
  -0.532529;0.574116;-0.621936;,
  0.664144;0.702036;-0.257018;,
  0.207218;0.952305;-0.224001;,
  -0.207218;0.952305;-0.224001;,
  -0.664144;0.702036;-0.257018;,
  0.664144;0.702035;0.257019;,
  0.207218;0.952304;0.224001;,
  -0.207218;0.952304;0.224001;,
  -0.664144;0.702035;0.257019;,
  0.532531;0.574116;0.621934;,
  0.210717;0.669904;0.711918;,
  -0.210717;0.669904;0.711918;,
  -0.532531;0.574116;0.621934;,
  0.633458;0.202883;0.746706;,
  0.232227;0.188558;0.954210;,
  -0.232227;0.188558;0.954210;,
  -0.633458;0.202883;0.746706;,
  0.606027;-0.339920;0.719156;,
  0.220616;-0.338447;0.914758;,
  -0.220616;-0.338447;0.914758;,
  -0.606027;-0.339920;0.719156;,
  0.470742;-0.680598;0.561416;,
  0.198281;-0.728600;0.655612;,
  -0.198281;-0.728600;0.655612;,
  -0.470742;-0.680598;0.561416;,
  0.614923;-0.746623;0.253820;,
  0.253907;-0.923669;0.286996;,
  -0.253907;-0.923669;0.286996;,
  -0.614923;-0.746623;0.253820;,
  0.614922;-0.746622;-0.253822;,
  0.253907;-0.923668;-0.286998;,
  -0.253908;-0.923668;-0.286998;,
  -0.614923;-0.746622;-0.253822;,
  -0.890031;-0.345286;0.297695;,
  -0.890030;-0.345287;-0.297697;,
  -0.925135;0.221744;0.308146;,
  -0.925135;0.221744;-0.308147;,
  0.890031;-0.345286;0.297695;,
  0.890030;-0.345287;-0.297697;,
  0.925135;0.221744;0.308146;,
  0.925135;0.221744;-0.308147;;
  54;
  4;0,1,5,4;,
  4;1,2,6,5;,
  4;2,3,7,6;,
  4;4,5,9,8;,
  4;5,6,10,9;,
  4;6,7,11,10;,
  4;8,9,13,12;,
  4;9,10,14,13;,
  4;10,11,15,14;,
  4;12,13,17,16;,
  4;13,14,18,17;,
  4;14,15,19,18;,
  4;16,17,21,20;,
  4;17,18,22,21;,
  4;18,19,23,22;,
  4;20,21,25,24;,
  4;21,22,26,25;,
  4;22,23,27,26;,
  4;24,25,29,28;,
  4;25,26,30,29;,
  4;26,27,31,30;,
  4;28,29,33,32;,
  4;29,30,34,33;,
  4;30,31,35,34;,
  4;32,33,37,36;,
  4;33,34,38,37;,
  4;34,35,39,38;,
  4;36,37,41,40;,
  4;37,38,42,41;,
  4;38,39,43,42;,
  4;40,41,45,44;,
  4;41,42,46,45;,
  4;42,43,47,46;,
  4;44,45,1,0;,
  4;45,46,2,1;,
  4;46,47,3,2;,
  4;43,39,35,48;,
  4;47,43,48,49;,
  4;3,47,49,7;,
  4;48,35,31,50;,
  4;49,48,50,51;,
  4;7,49,51,11;,
  4;50,31,27,23;,
  4;51,50,23,19;,
  4;11,51,19,15;,
  4;36,40,52,32;,
  4;40,44,53,52;,
  4;44,0,4,53;,
  4;32,52,54,28;,
  4;52,53,55,54;,
  4;53,4,8,55;,
  4;28,54,20,24;,
  4;54,55,16,20;,
  4;55,8,12,16;;
 }
 MeshTextureCoords {
  56;
  0.899161;0.903916;,
  0.907033;0.903916;,
  0.904738;0.894881;,
  0.894591;0.894881;,
  0.922920;0.903916;,
  0.925215;0.894881;,
  0.930792;0.903916;,
  0.935362;0.894881;,
  0.904738;0.703126;,
  0.894591;0.703126;,
  0.925215;0.703126;,
  0.935362;0.703126;,
  0.906145;0.693749;,
  0.897392;0.693749;,
  0.923808;0.693749;,
  0.932561;0.693749;,
  0.904738;0.685328;,
  0.888481;0.693749;,
  0.925215;0.685328;,
  0.941472;0.693749;,
  0.904738;0.685328;,
  0.888481;0.693749;,
  0.925215;0.685328;,
  0.941472;0.693749;,
  0.906145;0.693749;,
  0.897392;0.693749;,
  0.923808;0.693749;,
  0.932561;0.693749;,
  0.904738;0.703126;,
  0.894591;0.703126;,
  0.925215;0.703126;,
  0.935362;0.703126;,
  0.904738;0.894881;,
  0.894591;0.894881;,
  0.925215;0.894881;,
  0.935362;0.894881;,
  0.907033;0.903916;,
  0.899161;0.903916;,
  0.922920;0.903916;,
  0.930792;0.903916;,
  0.904738;0.912679;,
  0.891146;0.903916;,
  0.925215;0.912679;,
  0.938806;0.903916;,
  0.904738;0.912679;,
  0.891146;0.903916;,
  0.925215;0.912679;,
  0.938806;0.903916;,
  0.945692;0.894881;,
  0.945692;0.894881;,
  0.945692;0.703126;,
  0.945692;0.703126;,
  0.884261;0.894881;,
  0.884261;0.894881;,
  0.884261;0.703126;,
  0.884261;0.703126;;
 }
}
