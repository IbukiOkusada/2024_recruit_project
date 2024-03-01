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
 104;
 -20.10241;32.84689;4.64882;,
 20.10241;32.84689;4.64882;,
 20.10241;-31.40540;4.64882;,
 -20.10241;-31.40540;4.64882;,
 20.10241;32.84689;4.64882;,
 20.10241;32.84689;11.39144;,
 20.10241;-31.40540;11.39144;,
 20.10241;-31.40540;4.64882;,
 20.10241;32.84689;11.39144;,
 -20.10241;32.84689;11.39144;,
 -20.10241;-31.40540;11.39144;,
 20.10241;-31.40540;11.39144;,
 -20.10241;32.84689;11.39144;,
 -20.10241;32.84689;4.64882;,
 -20.10241;-31.40540;4.64882;,
 -20.10241;-31.40540;11.39144;,
 20.10241;32.84689;4.64882;,
 -20.10241;32.84689;4.64882;,
 -20.10241;-31.40540;4.64882;,
 20.10241;-31.40540;4.64882;,
 -20.56387;18.07236;6.69782;,
 -20.56387;18.07236;1.38160;,
 -20.56387;-25.65105;-0.36980;,
 -20.56387;-25.65105;6.69782;,
 -10.05121;18.07236;-6.82602;,
 0.46146;18.07236;-6.82602;,
 0.46146;-25.65105;-6.82602;,
 -10.05121;-25.65105;-6.82602;,
 -10.05121;18.07236;-6.82602;,
 -10.05121;18.07236;2.82971;,
 0.46146;18.07236;2.82971;,
 0.46146;18.07236;-6.82602;,
 -10.05121;-25.65105;-6.82602;,
 0.46146;-25.65105;-6.82602;,
 0.46146;-25.65105;2.82971;,
 -10.05121;-25.65105;2.82971;,
 -10.05121;18.07236;2.82971;,
 -10.05121;-25.65105;2.82971;,
 -15.30754;18.07236;-3.86245;,
 -15.30754;-25.65105;-3.86245;,
 -20.56387;-25.65105;-0.36980;,
 -20.56387;18.07236;1.38160;,
 -15.30754;18.07236;-3.86245;,
 -20.56387;18.07236;1.38160;,
 -15.30754;18.07236;5.79329;,
 -15.30754;-25.65105;-3.86245;,
 -15.30754;-25.65105;5.79329;,
 -20.56387;-25.65105;-0.36980;,
 -15.30754;18.07236;5.79329;,
 -20.56387;18.07236;6.69782;,
 -20.56387;-25.65105;6.69782;,
 -15.30754;-25.65105;5.79329;,
 20.56387;-25.65105;6.69782;,
 20.56387;-25.65105;-0.36980;,
 20.56387;18.07236;1.38160;,
 20.56387;18.07236;6.69782;,
 10.05121;-25.65105;-6.82602;,
 -0.46146;-25.65105;-6.82602;,
 -0.46146;18.07236;-6.82602;,
 10.05121;18.07236;-6.82602;,
 -0.46146;18.07236;-6.82602;,
 -0.46146;18.07236;2.82971;,
 10.05121;18.07236;2.82971;,
 10.05121;18.07236;-6.82602;,
 10.05121;-25.65105;2.82971;,
 -0.46146;-25.65105;2.82971;,
 -0.46146;-25.65105;-6.82602;,
 10.05121;-25.65105;-6.82602;,
 10.05121;-25.65105;2.82971;,
 10.05121;18.07236;2.82971;,
 15.30754;-25.65105;-3.86245;,
 15.30754;18.07236;-3.86245;,
 20.56387;18.07236;1.38160;,
 20.56387;-25.65105;-0.36980;,
 15.30754;18.07236;5.79329;,
 20.56387;18.07236;1.38160;,
 15.30754;18.07236;-3.86245;,
 15.30754;-25.65105;5.79329;,
 15.30754;-25.65105;-3.86245;,
 20.56387;-25.65105;-0.36980;,
 15.30754;-25.65105;5.79329;,
 20.56387;-25.65105;6.69782;,
 20.56387;18.07236;6.69782;,
 15.30754;18.07236;5.79329;,
 -10.00000;32.18814;3.35950;,
 10.00000;32.18814;3.35950;,
 10.00000;24.77534;3.35950;,
 -10.00000;24.77534;3.35950;,
 10.00000;32.18814;3.35950;,
 10.00000;32.18814;7.95192;,
 10.00000;24.77534;7.95192;,
 10.00000;24.77534;3.35950;,
 10.00000;32.18814;7.95192;,
 -10.00000;32.18814;7.95192;,
 -10.00000;24.77534;7.95192;,
 10.00000;24.77534;7.95192;,
 -10.00000;32.18814;7.95192;,
 -10.00000;32.18814;3.35950;,
 -10.00000;24.77534;3.35950;,
 -10.00000;24.77534;7.95192;,
 10.00000;32.18814;3.35950;,
 -10.00000;32.18814;3.35950;,
 -10.00000;24.77534;3.35950;,
 10.00000;24.77534;3.35950;;
 
 38;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;12,5,16,17;,
 4;18,19,6,15;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;36,37,34,30;,
 4;38,24,27,39;,
 4;38,39,40,41;,
 4;42,43,20,44;,
 4;42,44,29,28;,
 4;45,32,35,46;,
 4;45,46,23,47;,
 4;48,49,50,51;,
 4;48,51,37,36;,
 4;52,53,54,55;,
 4;56,57,58,59;,
 4;60,61,62,63;,
 4;64,65,66,67;,
 4;61,65,68,69;,
 4;70,56,59,71;,
 4;72,73,70,71;,
 4;74,55,75,76;,
 4;63,62,74,76;,
 4;77,64,67,78;,
 4;79,52,77,78;,
 4;80,81,82,83;,
 4;69,68,80,83;,
 4;84,85,86,87;,
 4;88,89,90,91;,
 4;92,93,94,95;,
 4;96,97,98,99;,
 4;96,89,100,101;,
 4;102,103,90,99;;
 
 MeshMaterialList {
  2;
  38;
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.109600;0.200800;0.310400;1.000000;;
   39.000000;
   0.400000;0.400000;0.400000;;
   0.086310;0.158130;0.244440;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  30;
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.903567;0.007882;-0.428374;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.253882;0.000000;-0.967235;,
  -0.564095;0.007149;-0.825679;,
  0.903567;0.007882;-0.428374;,
  1.000000;0.000000;0.000000;,
  0.253882;0.000000;-0.967235;,
  0.564095;0.007149;-0.825679;,
  0.253883;0.000000;0.967235;,
  0.000000;-0.000000;1.000000;,
  0.335277;0.000000;0.942119;,
  0.169590;0.000000;0.985515;,
  -0.253883;0.000000;0.967235;,
  -0.335277;0.000000;0.942119;,
  -0.169590;0.000000;0.985515;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;;
  38;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;7,6,6,7;,
  4;11,8,8,11;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;17,17,18,18;,
  4;12,11,11,12;,
  4;12,12,6,6;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;19,20,20,19;,
  4;19,19,17,17;,
  4;14,13,13,14;,
  4;15,8,8,15;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;18,18,21,21;,
  4;16,15,15,16;,
  4;13,13,16,16;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;22,23,23,22;,
  4;21,21,22,22;,
  4;24,24,24,24;,
  4;25,25,25,25;,
  4;26,26,26,26;,
  4;27,27,27,27;,
  4;28,28,28,28;,
  4;29,29,29,29;;
 }
 MeshTextureCoords {
  104;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.250000;1.000000;,
  0.250000;1.000000;,
  0.250000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.250000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.250000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.125000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;0.000000;,
  0.875000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.875000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.250000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.250000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.250000;0.000000;,
  0.750000;1.000000;,
  0.750000;0.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.125000;0.000000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.875000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.875000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}
