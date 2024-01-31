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
 44;
 7.64754;26.17585;-3.73455;,
 8.29963;17.07722;-5.76680;,
 -8.88408;17.07722;-5.76680;,
 -8.15375;26.17585;-3.73455;,
 7.64754;26.17585;3.73455;,
 7.64754;26.17585;0.00000;,
 -8.15375;26.17585;0.00000;,
 -8.15375;26.17585;3.73455;,
 -8.88408;22.90103;5.38498;,
 8.29963;22.90103;5.38498;,
 -8.88408;0.08521;0.00000;,
 8.29963;0.08521;0.00000;,
 8.29963;0.08521;3.73455;,
 -8.88408;0.08521;3.73455;,
 -10.34594;26.17585;0.00000;,
 -12.62440;22.90103;0.00000;,
 -11.07627;22.90103;3.73455;,
 -10.34594;26.17585;3.73455;,
 10.02574;22.90103;3.73455;,
 11.38518;22.90103;0.00000;,
 9.37365;26.17585;0.00000;,
 9.37365;26.17585;3.73455;,
 9.63201;0.08521;3.73455;,
 9.63201;0.08521;0.00000;,
 11.38518;22.90103;-3.73455;,
 9.37365;26.17585;-3.73455;,
 -11.80227;22.90103;-3.73455;,
 -10.34594;26.17585;-3.73455;,
 -10.45576;0.08521;0.00000;,
 -10.22886;0.08521;2.90076;,
 11.38518;2.72439;-3.73455;,
 8.29963;2.72439;-7.02295;,
 -8.88408;2.72439;-7.02295;,
 -11.80227;2.72439;-3.73455;,
 -12.62440;2.72439;0.00000;,
 -11.07627;2.72439;3.73455;,
 -8.88408;2.72439;6.72449;,
 8.29963;2.72439;6.72449;,
 9.45937;2.72439;3.73455;,
 10.81882;2.72439;0.00000;,
 9.45516;0.08521;-3.73455;,
 8.29963;0.08521;-3.73455;,
 -8.88408;0.08521;-3.73455;,
 -10.22094;0.08521;-3.73455;;
 
 42;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,4,7;,
 4;10,11,12,13;,
 4;14,15,16,17;,
 4;18,19,20,21;,
 4;22,12,11,23;,
 4;21,4,9,18;,
 4;20,5,4,21;,
 4;24,1,0,25;,
 4;3,2,26,27;,
 4;7,6,14,17;,
 4;16,8,7,17;,
 4;28,10,13,29;,
 4;30,31,1,24;,
 4;2,1,31,32;,
 4;26,2,32,33;,
 4;15,34,35,16;,
 4;35,36,8,16;,
 4;36,37,9,8;,
 4;18,9,37,38;,
 4;38,39,19,18;,
 4;40,41,31,30;,
 4;32,31,41,42;,
 4;33,32,42,43;,
 4;34,28,29,35;,
 4;13,36,35,29;,
 4;12,37,36,13;,
 4;38,37,12,22;,
 4;22,23,39,38;,
 4;25,0,5,20;,
 4;6,5,0,3;,
 4;14,6,3,27;,
 4;26,15,14,27;,
 4;33,34,15,26;,
 4;43,28,34,33;,
 4;42,10,28,43;,
 4;41,11,10,42;,
 4;23,11,41,40;,
 4;39,23,40,30;,
 4;19,39,30,24;,
 4;20,19,24,25;;
 
 MeshMaterialList {
  6;
  42;
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4;;
  Material {
   0.400000;0.400000;0.400000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\UV\\robot_UV.png";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  46;
  0.471792;-0.847964;-0.241597;,
  -0.449619;-0.842650;-0.296284;,
  0.145143;0.199328;-0.969124;,
  -0.104051;0.115924;-0.987793;,
  0.725360;0.378691;0.574844;,
  -0.692469;0.397534;0.602041;,
  0.000000;-1.000000;0.000000;,
  -0.543940;-0.771660;0.329650;,
  0.093178;-0.933080;-0.347390;,
  0.000000;-0.972240;0.233985;,
  0.000000;1.000000;0.000000;,
  0.072767;0.209219;-0.975158;,
  -0.100643;-0.930691;-0.351690;,
  -0.052165;0.167403;-0.984507;,
  0.411159;0.119587;-0.903685;,
  0.206154;0.139712;-0.968494;,
  -0.201457;0.118114;-0.972350;,
  -0.665769;0.055384;-0.744100;,
  -0.783528;0.197832;0.589022;,
  -0.275363;0.226567;0.934260;,
  0.331546;0.230612;0.914820;,
  0.833713;0.177138;0.523014;,
  0.852701;-0.345815;-0.391552;,
  0.284983;-0.384138;-0.878193;,
  -0.298484;-0.377614;-0.876536;,
  -0.801287;-0.337110;-0.494262;,
  -0.813634;-0.263377;0.518298;,
  -0.373814;-0.331444;0.866261;,
  0.394542;-0.264989;0.879840;,
  0.861690;-0.111968;0.494927;,
  0.893094;0.443299;0.076606;,
  -0.895121;0.444141;0.038688;,
  -0.970467;0.232420;0.064615;,
  -0.953133;-0.293850;0.072044;,
  -0.468548;-0.882979;0.028474;,
  0.970061;-0.182975;0.159691;,
  0.964568;0.218418;0.147993;,
  0.189326;0.391692;0.900407;,
  -0.160993;0.385931;0.908371;,
  -0.586878;-0.487479;0.646482;,
  0.528052;-0.232042;0.816895;,
  0.816646;-0.218332;0.534248;,
  0.934115;-0.341567;0.103736;,
  0.488164;0.872752;0.000000;,
  -0.868063;0.488289;-0.089668;,
  0.963338;0.265417;0.039156;;
  42;
  4;11,15,16,13;,
  4;10,10,10,10;,
  4;19,20,37,38;,
  4;6,6,9,9;,
  4;31,32,18,5;,
  4;21,36,30,4;,
  4;6,9,6,6;,
  4;4,37,20,21;,
  4;10,10,10,10;,
  4;14,15,11,2;,
  4;13,16,17,3;,
  4;10,10,10,10;,
  4;18,19,38,5;,
  4;34,6,9,7;,
  4;22,23,15,14;,
  4;16,15,23,24;,
  4;17,16,24,25;,
  4;32,33,26,18;,
  4;26,27,19,18;,
  4;27,28,20,19;,
  4;21,20,28,29;,
  4;29,35,36,21;,
  4;0,8,23,22;,
  4;24,23,8,12;,
  4;25,24,12,1;,
  4;33,34,7,26;,
  4;39,27,26,7;,
  4;9,28,27,9;,
  4;29,28,40,41;,
  4;41,42,35,29;,
  4;43,10,10,10;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;44,32,31,44;,
  4;25,33,32,17;,
  4;1,34,33,25;,
  4;12,6,34,1;,
  4;8,6,6,12;,
  4;6,6,8,0;,
  4;35,42,0,22;,
  4;36,35,22,45;,
  4;30,36,45,43;;
 }
 MeshTextureCoords {
  44;
  0.572324;0.145553;,
  0.564297;0.248618;,
  0.775818;0.248618;,
  0.766828;0.145553;,
  0.572324;0.145553;,
  0.572324;0.145553;,
  0.766828;0.145553;,
  0.766828;0.145553;,
  0.775818;0.182649;,
  0.564297;0.182649;,
  0.775818;0.441096;,
  0.564297;0.441096;,
  0.564297;0.441096;,
  0.775818;0.441096;,
  0.793813;0.145553;,
  0.821859;0.182649;,
  0.802802;0.182649;,
  0.793813;0.145553;,
  0.543050;0.182649;,
  0.526316;0.182649;,
  0.551077;0.145553;,
  0.551077;0.145553;,
  0.547896;0.441096;,
  0.547896;0.441096;,
  0.526316;0.182649;,
  0.551077;0.145553;,
  0.811739;0.182649;,
  0.793813;0.145553;,
  0.795164;0.441096;,
  0.792371;0.441096;,
  0.526316;0.411201;,
  0.564297;0.411201;,
  0.775818;0.411201;,
  0.811739;0.411201;,
  0.821859;0.411201;,
  0.802802;0.411201;,
  0.775818;0.411201;,
  0.564297;0.411201;,
  0.550021;0.411201;,
  0.533287;0.411201;,
  0.550073;0.441096;,
  0.564297;0.441096;,
  0.775818;0.441096;,
  0.792274;0.441096;;
 }
}
