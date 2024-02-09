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
 58;
 -5.97008;0.94095;0.92517;,
 -5.97008;-0.04040;1.30839;,
 -5.26742;-0.04040;2.41760;,
 -5.26742;1.77290;1.70950;,
 -5.97008;-1.02175;0.92517;,
 -5.26742;-1.85370;1.70950;,
 -5.97008;-1.42824;0.00000;,
 -5.26742;-2.60479;0.00000;,
 -5.97008;-1.02175;-0.92517;,
 -5.26742;-1.85370;-1.70950;,
 -5.97008;-0.04040;-1.30839;,
 -5.26742;-0.04040;-2.41760;,
 -5.97008;0.94095;-0.92517;,
 -5.26742;1.77290;-1.70950;,
 -5.97008;1.34744;0.00000;,
 -5.26742;2.52400;0.00000;,
 -4.21582;-0.04040;3.15874;,
 -4.21582;2.32880;2.23357;,
 -4.21582;-2.40959;2.23357;,
 -4.21582;-3.39094;0.00000;,
 -4.21582;-2.40959;-2.23357;,
 -4.21582;-0.04040;-3.15874;,
 -4.21582;2.32880;-2.23357;,
 -4.21582;3.31015;0.00000;,
 -2.97537;-0.04040;3.41900;,
 -2.97537;2.52400;2.41760;,
 -2.97537;-2.60479;2.41760;,
 -2.97537;-3.66700;0.00000;,
 -2.97537;-2.60479;-2.41760;,
 -2.97537;-0.04040;-3.41900;,
 -2.97537;2.52400;-2.41760;,
 -2.97537;3.58620;0.00000;,
 -1.73492;-0.04040;3.15874;,
 -1.73492;2.32880;2.23357;,
 -1.73492;-2.40959;2.23357;,
 -1.73492;-3.39094;0.00000;,
 -1.73492;-2.40959;-2.23357;,
 -1.73492;-0.04040;-3.15874;,
 -1.73492;2.32880;-2.23357;,
 -1.73492;3.31015;0.00000;,
 -0.68331;-0.04040;2.41760;,
 -0.68331;1.77290;1.70950;,
 -0.68331;-1.85370;1.70950;,
 -0.68331;-2.60479;0.00000;,
 -0.68331;-1.85370;-1.70950;,
 -0.68331;-0.04040;-2.41760;,
 -0.68331;1.77290;-1.70950;,
 -0.68331;2.52400;0.00000;,
 0.01934;-0.04040;1.30839;,
 0.01934;0.94095;0.92517;,
 0.01934;-1.02175;0.92517;,
 0.01934;-1.42824;-0.00000;,
 0.01934;-1.02175;-0.92517;,
 0.01934;-0.04040;-1.30839;,
 0.01934;0.94095;-0.92517;,
 0.01934;1.34744;0.00000;,
 -6.21682;-0.04040;0.00000;,
 0.26609;-0.04040;-0.00000;;
 
 64;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,0,3,15;,
 4;3,2,16,17;,
 4;2,5,18,16;,
 4;5,7,19,18;,
 4;7,9,20,19;,
 4;9,11,21,20;,
 4;11,13,22,21;,
 4;13,15,23,22;,
 4;15,3,17,23;,
 4;17,16,24,25;,
 4;16,18,26,24;,
 4;18,19,27,26;,
 4;19,20,28,27;,
 4;20,21,29,28;,
 4;21,22,30,29;,
 4;22,23,31,30;,
 4;23,17,25,31;,
 4;25,24,32,33;,
 4;24,26,34,32;,
 4;26,27,35,34;,
 4;27,28,36,35;,
 4;28,29,37,36;,
 4;29,30,38,37;,
 4;30,31,39,38;,
 4;31,25,33,39;,
 4;33,32,40,41;,
 4;32,34,42,40;,
 4;34,35,43,42;,
 4;35,36,44,43;,
 4;36,37,45,44;,
 4;37,38,46,45;,
 4;38,39,47,46;,
 4;39,33,41,47;,
 4;41,40,48,49;,
 4;40,42,50,48;,
 4;42,43,51,50;,
 4;43,44,52,51;,
 4;44,45,53,52;,
 4;45,46,54,53;,
 4;46,47,55,54;,
 4;47,41,49,55;,
 3;1,0,56;,
 3;4,1,56;,
 3;6,4,56;,
 3;8,6,56;,
 3;10,8,56;,
 3;12,10,56;,
 3;14,12,56;,
 3;0,14,56;,
 3;49,48,57;,
 3;48,50,57;,
 3;50,51,57;,
 3;51,52,57;,
 3;52,53,57;,
 3;53,54,57;,
 3;54,55,57;,
 3;55,49,57;;
 
 MeshMaterialList {
  5;
  64;
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
  58;
  -0.934264;0.245135;0.258958;,
  -0.930969;0.000000;0.365098;,
  -0.934264;-0.245136;0.258957;,
  -0.937590;-0.347744;0.000000;,
  -0.934264;-0.245136;-0.258957;,
  -0.930969;0.000000;-0.365098;,
  -0.934264;0.245135;-0.258958;,
  -0.937590;0.347742;0.000000;,
  -0.735444;0.467080;0.490875;,
  -0.726160;-0.000000;0.687526;,
  -0.735443;-0.467082;0.490875;,
  -0.744983;-0.667084;0.000000;,
  -0.735443;-0.467082;-0.490875;,
  -0.726160;-0.000000;-0.687526;,
  -0.735444;0.467080;-0.490875;,
  -0.744985;0.667081;0.000000;,
  -0.410084;0.630410;0.659101;,
  -0.400805;-0.000001;0.916163;,
  -0.410083;-0.630411;0.659101;,
  -0.419830;-0.907603;0.000000;,
  -0.410083;-0.630411;-0.659101;,
  -0.400805;-0.000001;-0.916163;,
  -0.410084;0.630410;-0.659101;,
  -0.419831;0.907602;0.000000;,
  -0.000001;0.691979;0.721918;,
  -0.000000;-0.000001;1.000000;,
  -0.000001;-0.691979;0.721918;,
  -0.000001;-1.000000;0.000000;,
  -0.000001;-0.691979;-0.721918;,
  -0.000000;-0.000001;-1.000000;,
  -0.000001;0.691979;-0.721918;,
  -0.000001;1.000000;0.000000;,
  0.410081;0.630411;0.659102;,
  0.400803;-0.000001;0.916164;,
  0.410081;-0.630412;0.659102;,
  0.419828;-0.907604;-0.000000;,
  0.410081;-0.630412;-0.659102;,
  0.400803;-0.000001;-0.916164;,
  0.410081;0.630411;-0.659102;,
  0.419828;0.907604;0.000000;,
  0.735445;0.467080;0.490874;,
  0.726161;-0.000000;0.687525;,
  0.735444;-0.467081;0.490874;,
  0.744984;-0.667083;-0.000000;,
  0.735444;-0.467081;-0.490874;,
  0.726161;-0.000000;-0.687525;,
  0.735445;0.467080;-0.490874;,
  0.744986;0.667080;0.000000;,
  0.934265;0.245134;0.258957;,
  0.930970;0.000000;0.365097;,
  0.934265;-0.245135;0.258957;,
  0.937590;-0.347743;-0.000000;,
  0.934265;-0.245135;-0.258957;,
  0.930970;0.000000;-0.365097;,
  0.934265;0.245134;-0.258957;,
  0.937591;0.347741;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;-0.000000;;
  64;
  4;0,1,9,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,0,8,15;,
  4;8,9,17,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,8,16,23;,
  4;16,17,25,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,16,24,31;,
  4;24,25,33,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,24,32,39;,
  4;32,33,41,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,32,40,47;,
  4;40,41,49,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,40,48,55;,
  3;1,0,56;,
  3;2,1,56;,
  3;3,2,56;,
  3;4,3,56;,
  3;5,4,56;,
  3;6,5,56;,
  3;7,6,56;,
  3;0,7,56;,
  3;48,49,57;,
  3;49,50,57;,
  3;50,51,57;,
  3;51,52,57;,
  3;52,53,57;,
  3;53,54,57;,
  3;54,55,57;,
  3;55,48,57;;
 }
 MeshTextureCoords {
  58;
  0.985295;0.304177;,
  0.985295;0.318221;,
  0.975617;0.318221;,
  0.975617;0.292270;,
  0.985295;0.332266;,
  0.975617;0.344172;,
  0.985295;0.338083;,
  0.975617;0.354921;,
  0.985295;0.332266;,
  0.975617;0.344172;,
  0.985295;0.318221;,
  0.975617;0.318221;,
  0.985295;0.304177;,
  0.975617;0.292270;,
  0.985295;0.298359;,
  0.975617;0.281521;,
  0.961133;0.318221;,
  0.961133;0.284315;,
  0.961133;0.352127;,
  0.961133;0.366172;,
  0.961133;0.352127;,
  0.961133;0.318221;,
  0.961133;0.284315;,
  0.961133;0.270270;,
  0.944048;0.318221;,
  0.944048;0.281521;,
  0.944048;0.354921;,
  0.944048;0.370123;,
  0.944048;0.354921;,
  0.944048;0.318221;,
  0.944048;0.281521;,
  0.944048;0.266320;,
  0.926962;0.318221;,
  0.926962;0.284315;,
  0.926962;0.352127;,
  0.926962;0.366172;,
  0.926962;0.352127;,
  0.926962;0.318221;,
  0.926962;0.284315;,
  0.926962;0.270270;,
  0.912478;0.318221;,
  0.912478;0.292270;,
  0.912478;0.344172;,
  0.912478;0.354921;,
  0.912478;0.344172;,
  0.912478;0.318221;,
  0.912478;0.292270;,
  0.912478;0.281521;,
  0.902800;0.318221;,
  0.902800;0.304177;,
  0.902800;0.332266;,
  0.902800;0.338083;,
  0.902800;0.332266;,
  0.902800;0.318221;,
  0.902800;0.304177;,
  0.902800;0.298359;,
  0.988694;0.318221;,
  0.899402;0.318221;;
 }
}
