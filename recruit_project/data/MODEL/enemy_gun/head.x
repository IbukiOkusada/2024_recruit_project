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
 64;
 4.16740;11.47288;-5.75149;,
 4.16740;10.06099;-7.15369;,
 -4.49183;10.06099;-7.15369;,
 -4.49183;11.47288;-5.75149;,
 4.16740;11.47288;2.91672;,
 4.16740;11.90949;1.12647;,
 -4.49183;11.90949;1.12647;,
 -4.49183;11.47288;2.91672;,
 4.16740;10.06099;3.65171;,
 -4.49183;10.06099;3.65171;,
 -4.49183;-0.03012;1.12647;,
 4.16740;-0.03012;1.12647;,
 4.16740;-0.03012;2.91672;,
 -4.49183;-0.03012;2.91672;,
 -5.75149;11.47288;1.12647;,
 -6.20398;10.06099;1.12647;,
 -5.75149;10.06099;2.91672;,
 -5.11472;11.47288;2.25105;,
 5.75149;10.06099;2.91672;,
 6.59505;10.06099;1.12647;,
 5.75149;11.47288;1.12647;,
 5.11472;11.47288;2.25105;,
 4.16740;1.33418;-7.15369;,
 -4.49183;1.33418;-7.15369;,
 -6.20398;1.33418;1.12647;,
 -5.75149;1.33418;2.91672;,
 4.16740;1.33418;3.65171;,
 -4.49183;1.33418;3.65171;,
 5.75149;1.33418;2.91672;,
 6.59505;1.33418;1.12647;,
 4.16740;-0.03012;-5.75149;,
 -4.49183;-0.03012;-5.75149;,
 -5.75149;-0.03012;1.12647;,
 -5.11839;-0.03012;2.14985;,
 5.11839;-0.03012;2.14985;,
 5.75149;-0.03012;1.12647;,
 4.16740;-0.03012;2.91672;,
 4.16740;-0.03012;1.12647;,
 4.16740;1.33418;3.65171;,
 4.16740;10.06099;3.65171;,
 4.16740;11.47288;2.91672;,
 4.16740;11.90949;1.12647;,
 5.75149;10.06099;-5.75149;,
 5.11472;11.47288;-5.08583;,
 5.75149;1.33418;-5.75149;,
 5.11839;-0.03012;-4.98463;,
 -5.11839;-0.03012;-4.98463;,
 -5.75149;1.33418;-5.75149;,
 -5.75149;10.06099;-5.75149;,
 -5.11472;11.47288;-5.08583;,
 5.75149;11.47288;-4.26716;,
 4.16740;11.90949;-4.26716;,
 4.16740;11.90949;-4.26716;,
 -4.49183;11.90949;-4.26716;,
 -4.49183;11.90949;-4.26716;,
 -5.75149;11.47288;-4.26716;,
 -6.20398;10.06099;-4.26716;,
 -6.20398;1.33418;-4.26716;,
 -5.75149;-0.03012;-4.26716;,
 -4.49183;-0.03012;-4.26716;,
 4.16740;-0.03012;-4.26716;,
 5.75149;-0.03012;-4.26716;,
 6.59505;1.33418;-4.26716;,
 6.59505;10.06099;-4.26716;;
 
 54;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,4,7,9;,
 4;10,11,12,13;,
 4;14,15,16,17;,
 4;18,19,20,21;,
 4;1,22,23,2;,
 4;15,24,25,16;,
 4;26,8,9,27;,
 4;28,29,19,18;,
 4;22,30,31,23;,
 4;24,32,33,25;,
 4;12,26,27,13;,
 4;34,35,29,28;,
 4;34,36,37,35;,
 4;28,38,36,34;,
 4;18,39,38,28;,
 4;21,40,39,18;,
 4;20,41,40,21;,
 4;42,1,0,43;,
 4;44,22,1,42;,
 4;45,30,22,44;,
 4;27,9,16,25;,
 4;13,27,25,33;,
 4;32,10,13,33;,
 4;23,31,46,47;,
 4;2,23,47,48;,
 4;3,2,48,49;,
 4;7,6,14,17;,
 4;9,7,17,16;,
 4;50,51,41,20;,
 4;6,5,52,53;,
 4;14,6,54,55;,
 4;55,56,15,14;,
 4;56,57,24,15;,
 4;57,58,32,24;,
 4;58,59,10,32;,
 4;59,60,11,10;,
 4;35,37,60,61;,
 4;29,35,61,62;,
 4;19,29,62,63;,
 4;20,19,63,50;,
 4;43,0,51,50;,
 4;54,51,0,3;,
 4;55,54,3,49;,
 4;48,56,55,49;,
 4;47,57,56,48;,
 4;46,58,57,47;,
 4;31,59,58,46;,
 4;30,60,59,31;,
 4;61,60,30,45;,
 4;62,61,45,44;,
 4;63,62,44,42;,
 4;50,63,42,43;;
 
 MeshMaterialList {
  6;
  54;
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
    "data\\TEXTURE\\UV\\robotgun_UV.png";
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
  54;
  0.449663;-0.793683;-0.409719;,
  -0.530894;-0.767180;-0.359981;,
  0.513080;0.711084;-0.480736;,
  -0.607294;0.666337;-0.432652;,
  0.513140;0.706090;0.487979;,
  -0.609169;0.652642;0.450525;,
  0.454230;-0.779692;0.430994;,
  -0.713894;-0.463829;0.524612;,
  0.728058;0.283163;-0.624300;,
  -0.821982;0.257079;-0.508188;,
  -0.778594;0.234594;0.582028;,
  0.686367;0.268929;0.675705;,
  0.725090;-0.299473;-0.620130;,
  -0.819361;-0.272020;-0.504632;,
  -0.777741;-0.245711;0.578572;,
  0.684961;-0.285052;0.670503;,
  0.143349;-0.920579;-0.363297;,
  0.167992;-0.946948;0.273985;,
  0.231060;-0.264448;0.936311;,
  0.226513;0.251366;0.941014;,
  0.179718;0.798171;0.575000;,
  0.195548;0.844431;-0.498696;,
  0.329545;0.358768;-0.873319;,
  0.333909;-0.372733;-0.865780;,
  -0.281390;0.254312;0.925281;,
  -0.285700;-0.266214;0.920601;,
  -0.208161;-0.945986;0.248557;,
  -0.174256;-0.922352;-0.344822;,
  -0.395333;-0.373661;-0.839100;,
  -0.391223;0.360828;-0.846610;,
  -0.246126;0.838517;-0.486118;,
  -0.229967;0.794541;0.561978;,
  0.574993;0.802223;0.160688;,
  0.121550;0.986625;0.108609;,
  -0.155759;0.981325;0.112876;,
  -0.677374;0.718995;0.155601;,
  -0.972276;0.176797;0.153045;,
  -0.970335;-0.183432;0.157490;,
  -0.913214;-0.362423;0.186254;,
  0.000000;-1.000000;-0.000000;,
  0.460083;-0.880237;0.116222;,
  0.933739;-0.281398;0.221239;,
  0.937754;0.269865;0.218608;,
  0.566790;0.800059;-0.196607;,
  0.125430;0.983183;-0.132735;,
  -0.161248;0.977179;-0.138274;,
  -0.665797;0.722760;-0.185288;,
  -0.964856;0.190234;-0.181287;,
  -0.961809;-0.201234;-0.185548;,
  -0.891864;-0.397266;-0.216236;,
  0.445693;-0.883955;-0.141354;,
  0.920873;-0.285589;-0.265389;,
  0.925895;0.272890;-0.261246;,
  0.000000;-0.474283;0.880372;;
  54;
  4;21,22,29,30;,
  4;20,33,34,31;,
  4;19,20,31,24;,
  4;39,39,17,26;,
  4;35,36,10,5;,
  4;11,42,32,4;,
  4;22,23,28,29;,
  4;36,37,14,10;,
  4;18,19,24,25;,
  4;15,41,42,11;,
  4;23,16,27,28;,
  4;37,38,7,14;,
  4;53,18,25,53;,
  4;6,40,41,15;,
  4;6,17,39,40;,
  4;15,18,17,6;,
  4;11,19,18,15;,
  4;4,20,19,11;,
  4;32,33,20,4;,
  4;8,22,21,2;,
  4;12,23,22,8;,
  4;0,16,23,12;,
  4;25,24,10,14;,
  4;26,25,14,7;,
  4;39,39,26,39;,
  4;28,27,1,13;,
  4;29,28,13,9;,
  4;30,29,9,3;,
  4;31,34,35,5;,
  4;24,31,5,10;,
  4;43,44,33,32;,
  4;34,33,44,45;,
  4;35,34,45,46;,
  4;46,47,36,35;,
  4;47,48,37,36;,
  4;48,49,38,37;,
  4;39,39,39,39;,
  4;39,39,39,39;,
  4;40,39,39,50;,
  4;41,40,50,51;,
  4;42,41,51,52;,
  4;32,42,52,43;,
  4;2,21,44,43;,
  4;45,44,21,30;,
  4;46,45,30,3;,
  4;9,47,46,3;,
  4;13,48,47,9;,
  4;1,49,48,13;,
  4;27,39,39,1;,
  4;16,39,39,27;,
  4;50,39,16,0;,
  4;51,50,0,12;,
  4;52,51,12,8;,
  4;43,52,8,2;;
 }
 MeshTextureCoords {
  64;
  0.132903;0.070189;,
  0.143071;0.093233;,
  0.235804;0.093233;,
  0.246096;0.070189;,
  0.451174;0.070189;,
  0.467596;0.063063;,
  0.332538;0.063063;,
  0.348542;0.070189;,
  0.446286;0.093233;,
  0.353553;0.093233;,
  0.332538;0.257931;,
  0.467596;0.257931;,
  0.451174;0.257931;,
  0.348542;0.257931;,
  0.325667;0.070189;,
  0.323735;0.093233;,
  0.340438;0.093233;,
  0.339089;0.070189;,
  0.030601;0.093233;,
  0.049592;0.093233;,
  0.045788;0.070189;,
  0.031686;0.070189;,
  0.143071;0.235664;,
  0.235804;0.235664;,
  0.323735;0.235664;,
  0.340438;0.235664;,
  0.446286;0.235664;,
  0.353553;0.235664;,
  0.030601;0.235664;,
  0.049592;0.235664;,
  0.132903;0.257931;,
  0.246096;0.257931;,
  0.325667;0.257931;,
  0.338223;0.257931;,
  0.032571;0.257931;,
  0.045788;0.257931;,
  0.019115;0.257931;,
  0.035536;0.257931;,
  0.014226;0.235664;,
  0.014226;0.093233;,
  0.019115;0.070189;,
  0.035536;0.063063;,
  0.121561;0.093233;,
  0.119616;0.070189;,
  0.121561;0.235664;,
  0.118613;0.257931;,
  0.257088;0.257931;,
  0.253959;0.235664;,
  0.253959;0.093233;,
  0.256119;0.070189;,
  0.107890;0.070189;,
  0.117479;0.063063;,
  0.549539;0.063063;,
  0.692861;0.063063;,
  0.260802;0.063063;,
  0.267155;0.070189;,
  0.268918;0.093233;,
  0.268918;0.235664;,
  0.267155;0.257931;,
  0.260802;0.257931;,
  0.117479;0.257931;,
  0.107890;0.257931;,
  0.104409;0.235664;,
  0.104409;0.093233;;
 }
}
