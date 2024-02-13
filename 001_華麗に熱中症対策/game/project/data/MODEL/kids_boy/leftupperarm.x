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
 32;
 -5.01594;1.05636;-1.05030;,
 -5.61283;0.40945;-1.05030;,
 -5.61283;0.40945;1.11326;,
 -5.01594;1.05636;1.11326;,
 0.64050;1.05636;-1.05030;,
 0.64050;1.05636;1.11326;,
 0.85322;0.40945;1.11326;,
 0.85322;0.40945;-1.05030;,
 -5.01594;-2.25498;-1.05030;,
 0.64050;-2.25498;-1.05030;,
 0.64050;-2.25498;1.11326;,
 -5.01594;-2.25498;1.11326;,
 -5.01594;0.40945;1.74733;,
 0.64050;0.40945;1.74733;,
 0.64050;0.77392;1.49528;,
 -5.01594;0.77392;1.49528;,
 0.64050;0.40945;-1.67431;,
 -5.01594;0.40945;-1.67431;,
 -5.01594;0.77392;-1.34923;,
 0.64050;0.77392;-1.34923;,
 0.64050;-2.00759;-1.34923;,
 -5.01594;-2.00759;-1.34923;,
 0.64050;-2.00759;1.49528;,
 -5.01594;-2.00759;1.49528;,
 -5.01594;-1.70308;-1.67431;,
 -5.61283;-1.70308;-1.05030;,
 -5.61283;-1.70308;1.11326;,
 -5.01594;-1.70308;1.74733;,
 0.64050;-1.70308;1.74733;,
 0.85322;-1.70308;1.11326;,
 0.85322;-1.70308;-1.05030;,
 0.64050;-1.70308;-1.67431;;
 
 30;
 4;0,1,2,3;,
 4;4,0,3,5;,
 4;6,7,4,5;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,9,8,21;,
 4;19,4,7,16;,
 4;18,0,4,19;,
 4;17,1,0,18;,
 4;3,2,12,15;,
 4;5,3,15,14;,
 4;13,6,5,14;,
 4;11,10,22,23;,
 4;24,25,1,17;,
 4;2,1,25,26;,
 4;12,2,26,27;,
 4;27,28,13,12;,
 4;28,29,6,13;,
 4;29,30,7,6;,
 4;16,7,30,31;,
 4;31,24,17,16;,
 4;21,8,25,24;,
 4;26,25,8,11;,
 4;27,26,11,23;,
 4;23,22,28,27;,
 4;10,29,28,22;,
 4;9,30,29,10;,
 4;31,30,9,20;,
 4;20,21,24,31;;
 
 MeshMaterialList {
  1;
  30;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "data\\TEXTURE\\UV\\kids_boy_UV.png";
   }
  }
 }
 MeshNormals {
  44;
  -0.202276;-0.734950;-0.647250;,
  -0.222125;-0.728775;0.647725;,
  -0.216416;0.680356;-0.700200;,
  -0.236497;0.676122;0.697803;,
  0.000000;0.696887;-0.717181;,
  0.000000;0.695844;0.718193;,
  0.000000;-0.750460;-0.660916;,
  0.000000;-0.747151;0.664655;,
  -0.353884;-0.875073;-0.330171;,
  0.000000;-0.940849;-0.338826;,
  0.000000;0.929211;-0.369548;,
  -0.388508;0.848455;-0.359423;,
  -0.365325;-0.882434;0.296391;,
  -0.399182;0.857920;0.323461;,
  0.000000;0.949762;0.312974;,
  0.000000;-0.959001;0.283401;,
  -0.382385;0.346394;-0.856617;,
  -0.868494;0.346371;-0.354607;,
  -0.876913;0.336493;0.343215;,
  -0.394937;0.309563;0.864983;,
  0.000000;0.297926;0.954589;,
  0.977815;0.146324;0.149889;,
  0.975276;0.153839;-0.158650;,
  0.000000;0.356176;-0.934419;,
  -0.376626;-0.387312;-0.841512;,
  -0.853358;-0.384841;-0.351679;,
  -0.862228;-0.374095;0.341492;,
  -0.388791;-0.349860;0.852314;,
  0.000000;-0.338861;0.940837;,
  0.973909;-0.170028;0.150305;,
  0.970968;-0.178760;-0.158950;,
  0.934229;-0.170091;-0.313504;,
  0.940886;0.303695;-0.150011;,
  0.947708;0.288967;0.135456;,
  0.910492;0.288131;-0.296623;,
  0.938528;0.145621;-0.312987;,
  0.946186;0.131210;0.295830;,
  0.927544;0.260095;0.268351;,
  0.942652;-0.153356;0.296462;,
  0.000000;-0.397717;-0.917508;,
  0.932656;-0.334499;0.135145;,
  0.914598;-0.303029;0.267737;,
  0.924258;-0.351375;-0.149274;,
  0.894697;-0.335222;-0.295199;;
  30;
  4;11,17,18,13;,
  4;10,11,13,14;,
  4;21,22,32,33;,
  4;8,9,15,12;,
  4;19,20,5,3;,
  4;23,16,2,4;,
  4;6,9,8,0;,
  4;34,32,22,35;,
  4;2,11,10,4;,
  4;16,17,11,2;,
  4;13,18,19,3;,
  4;14,13,3,5;,
  4;36,21,33,37;,
  4;12,15,7,1;,
  4;24,25,17,16;,
  4;18,17,25,26;,
  4;19,18,26,27;,
  4;27,28,20,19;,
  4;38,29,21,36;,
  4;29,30,22,21;,
  4;35,22,30,31;,
  4;39,24,16,23;,
  4;0,8,25,24;,
  4;26,25,8,12;,
  4;27,26,12,1;,
  4;1,7,28,27;,
  4;40,29,38,41;,
  4;42,30,29,40;,
  4;31,30,42,43;,
  4;6,0,24,39;;
 }
 MeshTextureCoords {
  32;
  0.489313;0.833827;,
  0.489313;0.860025;,
  0.404519;0.860025;,
  0.404519;0.833827;,
  0.489313;0.833827;,
  0.404519;0.833827;,
  0.404519;0.860025;,
  0.489313;0.860025;,
  0.489313;0.967926;,
  0.489313;0.967926;,
  0.404519;0.967926;,
  0.404519;0.967926;,
  0.379669;0.860025;,
  0.379669;0.860025;,
  0.389547;0.845265;,
  0.389547;0.845265;,
  0.513769;0.860025;,
  0.513769;0.860025;,
  0.501028;0.845265;,
  0.501028;0.845265;,
  0.501028;0.957908;,
  0.501028;0.957908;,
  0.389547;0.957908;,
  0.389547;0.957908;,
  0.513769;0.945576;,
  0.489313;0.945576;,
  0.404519;0.945576;,
  0.379669;0.945576;,
  0.379669;0.945576;,
  0.404519;0.945576;,
  0.489313;0.945576;,
  0.513769;0.945576;;
 }
}
