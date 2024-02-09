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
 2.14356;-11.53709;-1.28667;,
 1.28978;-11.53709;-1.71931;,
 1.53870;-11.00714;-2.45616;,
 2.63918;-11.00714;-1.88808;,
 -0.43319;-11.53709;-1.71931;,
 -0.68211;-11.00714;-2.45616;,
 -1.28698;-11.53709;-1.28667;,
 -1.78259;-11.00714;-1.88808;,
 1.53870;0.23981;-2.45616;,
 2.63918;0.23981;-1.88808;,
 -0.68211;0.23981;-2.45616;,
 -1.78259;0.23981;-1.88808;,
 1.38613;0.78983;-2.09513;,
 2.33541;0.78983;-1.59341;,
 -0.52954;0.78983;-2.09513;,
 -1.47882;0.78983;-1.59341;,
 1.53870;1.28372;-0.39585;,
 3.30179;0.78983;-0.27551;,
 -0.68211;1.28372;-0.39585;,
 -2.44520;0.78983;-0.27551;,
 1.53870;1.28372;1.66446;,
 3.30179;0.78983;1.54411;,
 -0.68211;1.28372;1.66446;,
 -2.44520;0.78983;1.54411;,
 1.38613;0.78983;3.36373;,
 2.33541;0.78983;2.86201;,
 -0.52954;0.78983;3.36373;,
 -1.47882;0.78983;2.86201;,
 1.53870;0.23981;3.72477;,
 2.63918;0.23981;3.15668;,
 -0.68211;0.23981;3.72477;,
 -1.78259;0.23981;3.15668;,
 1.53870;-11.00714;3.72477;,
 2.63918;-11.00714;3.15668;,
 -0.68211;-11.00714;3.72477;,
 -1.78259;-11.00714;3.15668;,
 1.28978;-11.53709;2.98791;,
 2.14356;-11.53709;2.55527;,
 -0.43319;-11.53709;2.98791;,
 -1.28698;-11.53709;2.55527;,
 1.53870;-12.05104;1.66446;,
 3.01274;-11.53709;1.41884;,
 -0.68211;-12.05104;1.66446;,
 -2.15615;-11.53709;1.41884;,
 1.53870;-12.05104;-0.39585;,
 3.01274;-11.53709;-0.15023;,
 -0.68211;-12.05104;-0.39585;,
 -2.15615;-11.53709;-0.15023;,
 -2.90291;-11.00714;1.66446;,
 -2.90291;-11.00714;-0.39585;,
 -2.90291;0.23981;1.66446;,
 -2.90291;0.23981;-0.39585;,
 3.75950;-11.00714;1.66446;,
 3.75950;-11.00714;-0.39585;,
 3.75950;0.23981;1.66446;,
 3.75950;0.23981;-0.39585;;
 
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
  0.334728;-0.853883;-0.398549;,
  0.136403;-0.880919;-0.453184;,
  -0.136403;-0.880919;-0.453184;,
  -0.334729;-0.853883;-0.398549;,
  0.568550;-0.471589;-0.674058;,
  0.206699;-0.470541;-0.857826;,
  -0.206699;-0.470541;-0.857827;,
  -0.568551;-0.471589;-0.674058;,
  0.611473;0.323266;-0.722219;,
  0.224312;0.304501;-0.925723;,
  -0.224312;0.304501;-0.925723;,
  -0.611472;0.323266;-0.722219;,
  0.428764;0.750609;-0.502739;,
  0.158498;0.814585;-0.557969;,
  -0.158498;0.814585;-0.557969;,
  -0.428764;0.750609;-0.502739;,
  0.503028;0.842909;-0.190963;,
  0.123308;0.983334;-0.133604;,
  -0.123308;0.983334;-0.133604;,
  -0.503028;0.842909;-0.190963;,
  0.503027;0.842909;0.190964;,
  0.123309;0.983333;0.133605;,
  -0.123309;0.983333;0.133605;,
  -0.503027;0.842909;0.190963;,
  0.428764;0.750611;0.502738;,
  0.158499;0.814588;0.557965;,
  -0.158499;0.814588;0.557965;,
  -0.428764;0.750611;0.502738;,
  0.611473;0.323268;0.722218;,
  0.224314;0.304506;0.925721;,
  -0.224314;0.304506;0.925721;,
  -0.611473;0.323268;0.722218;,
  0.568551;-0.471590;0.674057;,
  0.206702;-0.470544;0.857825;,
  -0.206701;-0.470544;0.857825;,
  -0.568552;-0.471589;0.674057;,
  0.334728;-0.853883;0.398549;,
  0.136403;-0.880920;0.453183;,
  -0.136403;-0.880920;0.453183;,
  -0.334729;-0.853883;0.398549;,
  0.420980;-0.890246;0.173888;,
  0.156106;-0.971507;0.178338;,
  -0.156105;-0.971507;0.178338;,
  -0.420981;-0.890246;0.173888;,
  0.420980;-0.890247;-0.173887;,
  0.156105;-0.971508;-0.178337;,
  -0.156105;-0.971508;-0.178336;,
  -0.420981;-0.890246;-0.173887;,
  -0.832523;-0.479054;0.278232;,
  -0.832523;-0.479054;-0.278231;,
  -0.889023;0.348298;0.297198;,
  -0.889024;0.348298;-0.297197;,
  0.832522;-0.479055;0.278232;,
  0.832523;-0.479055;-0.278231;,
  0.889023;0.348299;0.297198;,
  0.889024;0.348298;-0.297197;;
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
  0.880045;0.649909;,
  0.891805;0.649909;,
  0.888376;0.642325;,
  0.873219;0.642325;,
  0.915536;0.649909;,
  0.918964;0.642325;,
  0.927296;0.649909;,
  0.934122;0.642325;,
  0.888376;0.481366;,
  0.873219;0.481366;,
  0.918964;0.481366;,
  0.934122;0.481366;,
  0.890478;0.473495;,
  0.877403;0.473495;,
  0.916863;0.473495;,
  0.929938;0.473495;,
  0.888376;0.466426;,
  0.864092;0.473495;,
  0.918964;0.466426;,
  0.943248;0.473495;,
  0.888376;0.466426;,
  0.864092;0.473495;,
  0.918964;0.466426;,
  0.943248;0.473495;,
  0.890478;0.473495;,
  0.877403;0.473495;,
  0.916863;0.473495;,
  0.929938;0.473495;,
  0.888376;0.481366;,
  0.873219;0.481366;,
  0.918964;0.481366;,
  0.934122;0.481366;,
  0.888376;0.642325;,
  0.873219;0.642325;,
  0.918964;0.642325;,
  0.934122;0.642325;,
  0.891805;0.649909;,
  0.880045;0.649909;,
  0.915536;0.649909;,
  0.927296;0.649909;,
  0.888376;0.657265;,
  0.868074;0.649909;,
  0.918964;0.657265;,
  0.939267;0.649909;,
  0.888376;0.657265;,
  0.868074;0.649909;,
  0.918964;0.657265;,
  0.939267;0.649909;,
  0.949553;0.642325;,
  0.949553;0.642325;,
  0.949553;0.481366;,
  0.949553;0.481366;,
  0.857788;0.642325;,
  0.857788;0.642325;,
  0.857788;0.481366;,
  0.857788;0.481366;;
 }
}
