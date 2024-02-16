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
 45;
 9.16414;0.00014;-0.00000;,
 9.32113;-0.00316;-0.37907;,
 8.01903;-3.14668;-0.35163;,
 7.90799;-3.03236;0.02646;,
 9.70023;-0.00454;-0.53607;,
 8.28704;-3.41610;-0.50630;,
 10.07930;-0.00316;-0.37907;,
 8.55508;-3.68277;-0.34695;,
 10.23630;0.00014;-0.00000;,
 8.66613;-3.79049;0.03308;,
 10.07930;0.00345;0.37906;,
 8.55508;-3.67615;0.41117;,
 9.70023;0.00483;0.53608;,
 8.28704;-3.40673;0.56584;,
 9.32113;0.00345;0.37906;,
 8.01903;-3.14006;0.40649;,
 9.16414;0.00014;-0.00000;,
 7.90799;-3.03236;0.02646;,
 4.87534;-4.44879;-0.34027;,
 4.87534;-4.28847;0.03742;,
 4.87534;-4.82922;-0.49397;,
 4.87534;-5.20691;-0.33365;,
 4.87534;-5.36062;0.04678;,
 4.87534;-5.20030;0.42448;,
 4.87534;-4.81986;0.57818;,
 4.87534;-4.44217;0.41786;,
 4.87534;-4.28847;0.03742;,
 1.73168;-3.14668;-0.35163;,
 1.84272;-3.03236;0.02646;,
 1.46367;-3.41610;-0.50630;,
 1.19564;-3.68277;-0.34695;,
 1.08458;-3.79049;0.03308;,
 1.19564;-3.67615;0.41117;,
 1.46367;-3.40673;0.56584;,
 1.73168;-3.14006;0.40650;,
 1.84272;-3.03236;0.02646;,
 0.42956;-0.00316;-0.37907;,
 0.58657;0.00014;-0.00000;,
 0.05048;-0.00454;-0.53608;,
 -0.32858;-0.00316;-0.37907;,
 -0.48558;0.00014;-0.00000;,
 -0.32858;0.00345;0.37906;,
 0.05048;0.00483;0.53608;,
 0.42956;0.00345;0.37906;,
 0.58657;0.00014;-0.00000;;
 
 64;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;3,2,18,19;,
 4;2,5,20,18;,
 4;5,7,21,20;,
 4;7,9,22,21;,
 4;9,11,23,22;,
 4;11,13,24,23;,
 4;13,15,25,24;,
 4;15,17,26,25;,
 4;19,18,27,28;,
 4;18,20,29,27;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;25,26,35,34;,
 4;28,27,36,37;,
 4;27,29,38,36;,
 4;29,30,39,38;,
 4;30,31,40,39;,
 4;31,32,41,40;,
 4;32,33,42,41;,
 4;33,34,43,42;,
 4;34,35,44,43;,
 4;3,2,1,0;,
 4;2,5,4,1;,
 4;5,7,6,4;,
 4;7,9,8,6;,
 4;9,11,10,8;,
 4;11,13,12,10;,
 4;13,15,14,12;,
 4;15,17,16,14;,
 4;19,18,2,3;,
 4;18,20,5,2;,
 4;20,21,7,5;,
 4;21,22,9,7;,
 4;22,23,11,9;,
 4;23,24,13,11;,
 4;24,25,15,13;,
 4;25,26,17,15;,
 4;28,27,18,19;,
 4;27,29,20,18;,
 4;29,30,21,20;,
 4;30,31,22,21;,
 4;31,32,23,22;,
 4;32,33,24,23;,
 4;33,34,25,24;,
 4;34,35,26,25;,
 4;37,36,27,28;,
 4;36,38,29,27;,
 4;38,39,30,29;,
 4;39,40,31,30;,
 4;40,41,32,31;,
 4;41,42,33,32;,
 4;42,43,34,33;,
 4;43,44,35,34;;
 
 MeshMaterialList {
  3;
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
   0.000000;0.599200;0.135200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.542400;0.542400;0.542400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  80;
  0.923880;0.382668;-0.003340;,
  0.671318;0.272062;-0.689431;,
  -0.000003;-0.008729;-0.999962;,
  -0.671307;-0.284049;-0.684589;,
  -0.923880;-0.382667;0.003341;,
  -0.671307;-0.272059;0.689442;,
  -0.000003;0.008724;0.999962;,
  0.671318;0.284054;0.684577;,
  -0.923881;0.382665;-0.003339;,
  -0.671319;0.272061;-0.689430;,
  0.000002;-0.008729;-0.999962;,
  0.671311;-0.284052;-0.684585;,
  0.923879;-0.382670;0.003341;,
  0.671311;-0.272062;0.689437;,
  0.000002;0.008723;0.999962;,
  -0.671321;0.284052;0.684575;,
  -0.707109;0.707078;-0.006171;,
  -0.494182;0.487920;-0.719526;,
  0.000002;-0.008729;-0.999962;,
  0.494176;-0.500399;-0.710909;,
  0.707107;-0.707079;0.006171;,
  0.494176;-0.487913;0.719535;,
  0.000000;0.008727;0.999962;,
  -0.494184;0.500404;0.710899;,
  0.000002;0.999962;-0.008728;,
  0.000001;0.692607;-0.721315;,
  -0.000000;-0.008729;-0.999962;,
  -0.000001;-0.705081;-0.709127;,
  -0.000001;-0.999962;0.008726;,
  -0.000001;-0.692593;0.721328;,
  -0.000000;0.008730;0.999962;,
  0.000001;0.705092;0.709116;,
  0.707108;0.707078;-0.006172;,
  0.494183;0.487919;-0.719526;,
  -0.000002;-0.008729;-0.999962;,
  -0.494175;-0.500395;-0.710912;,
  -0.707108;-0.707078;0.006171;,
  -0.494175;-0.487909;0.719538;,
  -0.000001;0.008727;0.999962;,
  0.494184;0.500403;0.710900;,
  0.707109;-0.707078;0.006171;,
  0.494182;-0.487920;0.719526;,
  0.671319;-0.272061;0.689430;,
  0.923881;-0.382665;0.003339;,
  -0.000002;0.008729;0.999962;,
  -0.000002;0.008729;0.999962;,
  -0.494176;0.500399;0.710909;,
  -0.671311;0.284052;0.684585;,
  -0.707107;0.707079;-0.006171;,
  -0.923879;0.382670;-0.003341;,
  -0.494176;0.487913;-0.719535;,
  -0.671311;0.272062;-0.689437;,
  -0.000000;-0.008727;-0.999962;,
  -0.000002;-0.008723;-0.999962;,
  0.494184;-0.500404;-0.710899;,
  0.671321;-0.284052;-0.684575;,
  -0.000002;-0.999962;0.008728;,
  -0.000001;-0.692607;0.721315;,
  0.000000;0.008729;0.999962;,
  0.000001;0.705081;0.709127;,
  0.000001;0.999962;-0.008726;,
  0.000001;0.692593;-0.721328;,
  0.000000;-0.008730;-0.999962;,
  -0.000001;-0.705092;-0.709116;,
  -0.707108;-0.707078;0.006172;,
  -0.494183;-0.487919;0.719526;,
  0.000002;0.008729;0.999962;,
  0.494175;0.500395;0.710912;,
  0.707108;0.707078;-0.006171;,
  0.494175;0.487909;-0.719538;,
  0.000001;-0.008727;-0.999962;,
  -0.494184;-0.500403;-0.710900;,
  -0.923880;-0.382668;0.003340;,
  -0.671318;-0.272062;0.689431;,
  0.000003;0.008729;0.999962;,
  0.671307;0.284049;0.684589;,
  0.923880;0.382667;-0.003341;,
  0.671307;0.272059;-0.689442;,
  0.000003;-0.008724;-0.999962;,
  -0.671318;-0.284054;-0.684577;;
  64;
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
  4;32,33,1,0;,
  4;33,34,2,1;,
  4;34,35,3,2;,
  4;35,36,4,3;,
  4;36,37,5,4;,
  4;37,38,6,5;,
  4;38,39,7,6;,
  4;39,32,0,7;,
  4;40,41,42,43;,
  4;41,44,45,42;,
  4;44,46,47,45;,
  4;46,48,49,47;,
  4;48,50,51,49;,
  4;50,52,53,51;,
  4;52,54,55,53;,
  4;54,40,43,55;,
  4;56,57,41,40;,
  4;57,58,44,41;,
  4;58,59,46,44;,
  4;59,60,48,46;,
  4;60,61,50,48;,
  4;61,62,52,50;,
  4;62,63,54,52;,
  4;63,56,40,54;,
  4;64,65,57,56;,
  4;65,66,58,57;,
  4;66,67,59,58;,
  4;67,68,60,59;,
  4;68,69,61,60;,
  4;69,70,62,61;,
  4;70,71,63,62;,
  4;71,64,56,63;,
  4;72,73,65,64;,
  4;73,74,66,65;,
  4;74,75,67,66;,
  4;75,76,68,67;,
  4;76,77,69,68;,
  4;77,78,70,69;,
  4;78,79,71,70;,
  4;79,72,64,71;;
 }
 MeshTextureCoords {
  45;
  0.500000;0.000000;,
  0.500000;0.125000;,
  0.625000;0.125000;,
  0.625000;0.000000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.750000;0.125000;,
  0.750000;0.000000;,
  0.750000;0.250000;,
  0.750000;0.375000;,
  0.750000;0.500000;,
  0.750000;0.625000;,
  0.750000;0.750000;,
  0.750000;0.875000;,
  0.750000;1.000000;,
  0.875000;0.125000;,
  0.875000;0.000000;,
  0.875000;0.250000;,
  0.875000;0.375000;,
  0.875000;0.500000;,
  0.875000;0.625000;,
  0.875000;0.750000;,
  0.875000;0.875000;,
  0.875000;1.000000;,
  1.000000;0.125000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  1.000000;0.375000;,
  1.000000;0.500000;,
  1.000000;0.625000;,
  1.000000;0.750000;,
  1.000000;0.875000;,
  1.000000;1.000000;;
 }
}