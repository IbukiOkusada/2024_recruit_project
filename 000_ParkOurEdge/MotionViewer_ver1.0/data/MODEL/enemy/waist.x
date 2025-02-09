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
 16;
 2.81864;0.47864;-3.66978;,
 2.81864;-7.66498;-3.66978;,
 -2.80307;-7.66498;-3.66978;,
 -2.80307;0.47864;-3.66978;,
 3.13013;0.47864;3.66978;,
 -2.80307;0.47864;3.66978;,
 3.13013;-7.66498;3.66978;,
 -2.80307;-7.66498;3.66978;,
 -10.53451;-10.02921;-3.66978;,
 -10.53451;-10.02921;3.66978;,
 -9.78224;0.47864;3.66978;,
 -9.78224;0.47864;-3.66978;,
 11.34211;-10.02921;3.66978;,
 11.34211;-10.02921;-3.66978;,
 9.78224;0.47864;-3.66978;,
 9.78224;0.47864;3.66978;;
 
 14;
 4;0,1,2,3;,
 4;4,0,3,5;,
 4;6,4,5,7;,
 4;1,6,7,2;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;12,6,1,13;,
 4;15,4,6,12;,
 4;14,0,4,15;,
 4;13,1,0,14;,
 4;3,2,8,11;,
 4;5,3,11,10;,
 4;7,5,10,9;,
 4;2,7,9,8;;
 
 MeshMaterialList {
  6;
  14;
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
  9;
  0.989160;0.146839;0.000000;,
  -0.997447;0.071408;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;0.000000;,
  -0.137287;-0.990527;0.002863;,
  0.147838;-0.989012;-0.000000;,
  -0.271974;-0.962288;0.005672;,
  0.292427;-0.956288;-0.000000;;
  14;
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;3,3,3,3;,
  4;5,5,6,6;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;7,5,5,7;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;3,3,3,3;,
  4;6,6,8,8;;
 }
 MeshTextureCoords {
  16;
  0.908471;0.045621;,
  0.908471;0.145485;,
  0.941584;0.145485;,
  0.941584;0.045621;,
  0.906636;0.045621;,
  0.941584;0.045621;,
  0.906636;0.145485;,
  0.941584;0.145485;,
  0.987123;0.174477;,
  0.987123;0.174477;,
  0.982692;0.045621;,
  0.982692;0.045621;,
  0.858267;0.174477;,
  0.858267;0.174477;,
  0.867454;0.045621;,
  0.867454;0.045621;;
 }
}
