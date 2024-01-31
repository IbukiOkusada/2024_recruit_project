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
 -6.82658;3.27985;2.76798;,
 -6.82658;1.59879;3.11427;,
 -0.72597;1.59879;3.11427;,
 -0.72597;3.27985;3.11427;,
 -6.82658;3.27985;-2.76798;,
 -6.82658;3.27985;-1.86900;,
 -0.72597;3.58785;-1.86900;,
 -0.72597;3.27985;-3.11427;,
 -0.72597;1.59879;-3.11427;,
 -6.82658;1.59879;-3.11427;,
 -0.72597;-3.31234;-1.86900;,
 -6.82658;-2.97561;-1.86900;,
 -6.82658;-2.66220;-2.90653;,
 -0.72597;-2.97561;-3.11427;,
 1.03052;3.27985;-1.86900;,
 1.03052;1.59879;-1.86900;,
 0.33311;1.37627;-2.42779;,
 0.33311;2.79876;-2.42779;,
 -8.10744;1.37627;-2.42779;,
 -8.80485;1.59879;-1.86900;,
 -8.80485;2.70487;-1.86900;,
 -8.10744;2.79876;-2.42779;,
 -8.10744;-2.49453;-2.42779;,
 -8.80485;-1.99995;-1.86900;,
 -8.10744;1.37627;2.42779;,
 -8.10744;2.79876;2.42779;,
 -0.72597;3.27985;3.11427;,
 -0.72597;1.59879;3.11427;,
 0.33311;1.37627;2.42779;,
 0.33311;2.79876;2.42779;,
 1.03052;-2.97561;-1.86900;,
 0.33311;-2.49453;-2.42779;,
 -8.10744;-0.77360;2.42779;,
 -6.82658;-0.94187;3.11427;,
 -0.72597;-0.94187;3.11427;,
 -0.72597;-0.94187;3.11427;,
 0.33311;-0.77360;2.42779;,
 1.03052;-0.94187;-1.86900;,
 0.33311;-0.77360;-2.42779;,
 -0.72597;-0.94187;-3.11427;,
 -6.82658;-0.94187;-3.11427;,
 -8.10744;-0.77360;-2.42779;,
 -8.80485;-0.94187;-1.86900;,
 -8.10744;-2.49453;2.42779;,
 -6.82658;-2.66220;2.90653;,
 -0.72597;-2.97561;3.11427;,
 -0.72597;-2.97561;3.11427;,
 0.33311;-2.49453;2.42779;,
 -8.80485;2.70487;1.46321;,
 -6.82658;3.27985;1.46321;,
 -0.72597;3.58785;-1.86900;,
 -6.82658;3.27985;-1.86900;,
 -0.72597;3.58785;1.46321;,
 -0.72597;3.58785;1.46321;,
 1.03052;3.27985;1.46321;,
 1.03052;1.59879;1.46321;,
 1.03052;-0.94187;1.46321;,
 1.03052;-2.97561;1.46321;,
 -0.72597;-3.31234;1.46321;,
 -0.72597;-3.31234;1.46321;,
 -6.82658;-2.97561;1.46321;,
 -8.80485;-1.99995;1.46321;,
 -8.80485;-0.94187;1.46321;,
 -8.80485;1.59879;1.46321;;
 
 54;
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
 4;26,27,28,29;,
 4;7,6,14,17;,
 4;16,8,7,17;,
 4;30,10,13,31;,
 4;32,33,1,24;,
 4;2,1,33,34;,
 4;28,27,35,36;,
 4;15,37,38,16;,
 4;38,39,8,16;,
 4;39,40,9,8;,
 4;18,9,40,41;,
 4;41,42,19,18;,
 4;43,44,33,32;,
 4;34,33,44,45;,
 4;36,35,46,47;,
 4;37,30,31,38;,
 4;13,39,38,31;,
 4;12,40,39,13;,
 4;41,40,12,22;,
 4;22,23,42,41;,
 4;48,49,5,20;,
 4;50,51,49,52;,
 4;14,6,53,54;,
 4;54,55,15,14;,
 4;55,56,37,15;,
 4;56,57,30,37;,
 4;57,58,10,30;,
 4;59,60,11,10;,
 4;23,11,60,61;,
 4;42,23,61,62;,
 4;19,42,62,63;,
 4;20,19,63,48;,
 4;25,0,49,48;,
 4;52,49,0,3;,
 4;54,53,26,29;,
 4;28,55,54,29;,
 4;36,56,55,28;,
 4;47,57,56,36;,
 4;46,58,57,47;,
 4;44,60,59,45;,
 4;61,60,44,43;,
 4;62,61,43,32;,
 4;63,62,32,24;,
 4;48,63,24,25;;
 
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
    "data\\TEXTURE\\UV\\robotboss_UV.png";
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
  68;
  -0.635163;-0.029007;0.771833;,
  0.688880;0.000000;0.724876;,
  -0.627234;0.050008;0.777224;,
  0.688880;0.000000;0.724876;,
  -0.516977;0.048669;-0.854615;,
  0.553631;0.294816;-0.778830;,
  -0.342208;-0.939310;-0.024272;,
  0.554591;-0.289982;-0.779961;,
  -0.222256;-0.059346;0.973181;,
  -0.202608;-0.967347;-0.152281;,
  -0.175230;0.978611;-0.107774;,
  -0.217171;0.100665;0.970929;,
  0.275532;-0.031409;0.960779;,
  0.270513;0.053097;0.961251;,
  0.157580;0.933724;-0.321446;,
  0.148243;-0.902985;-0.403290;,
  -0.643116;0.024877;0.765364;,
  -0.230579;0.050709;0.971731;,
  0.277179;0.026487;0.960453;,
  0.688880;0.000000;0.724876;,
  0.585337;0.000000;-0.810790;,
  0.277179;0.026487;-0.960453;,
  -0.230579;0.050709;-0.971731;,
  -0.534376;0.024278;-0.844898;,
  -0.646854;-0.014450;0.762477;,
  -0.232958;-0.029821;0.972029;,
  0.279610;-0.015683;0.959985;,
  0.585337;0.000000;-0.810790;,
  0.279610;-0.015683;-0.959985;,
  -0.232958;-0.029821;-0.972029;,
  -0.538574;-0.014112;-0.842460;,
  -0.901466;0.000000;-0.432849;,
  -0.170798;0.983833;-0.053867;,
  0.110352;0.980878;-0.160318;,
  0.458575;0.722920;-0.516813;,
  0.901466;0.000000;-0.432850;,
  0.793228;-0.318652;-0.518893;,
  0.109654;-0.972841;-0.203853;,
  -0.228946;-0.970432;-0.076462;,
  -0.392874;-0.919512;-0.012155;,
  -0.297330;0.954393;0.026997;,
  -0.170427;0.984681;0.036861;,
  0.108116;0.987562;0.114158;,
  0.256706;0.949503;0.180404;,
  0.951413;0.000000;0.307919;,
  0.951413;0.000000;0.307919;,
  0.107219;-0.983423;0.146229;,
  -0.224466;-0.972544;0.061428;,
  -0.385935;-0.922356;0.017711;,
  -0.951413;0.000000;0.307918;,
  -0.217171;0.100665;-0.970929;,
  0.270513;0.053097;-0.961251;,
  -0.297198;0.953696;-0.046239;,
  -0.314550;0.944737;-0.092362;,
  0.275532;-0.031409;-0.960779;,
  -0.222256;-0.059346;-0.973181;,
  -0.525721;-0.028268;-0.850188;,
  1.000000;0.000000;0.000000;,
  0.269530;-0.944960;0.185485;,
  0.188278;-0.982116;-0.000000;,
  -0.315235;0.947478;0.053965;,
  -0.174755;0.981841;0.073816;,
  0.153983;0.961184;0.228941;,
  0.330211;0.875238;0.353438;,
  0.145497;-0.945437;0.291511;,
  0.339358;-0.867749;0.363109;,
  -0.194254;-0.973319;0.122131;,
  -0.327951;-0.944033;0.035349;;
  54;
  4;11,17,18,13;,
  4;10,32,33,14;,
  4;21,22,50,51;,
  4;37,38,9,15;,
  4;34,35,20,5;,
  4;23,31,31,4;,
  4;6,9,38,39;,
  4;4,50,22,23;,
  4;52,32,10,53;,
  4;16,17,11,2;,
  4;13,18,19,3;,
  4;14,33,34,5;,
  4;20,21,51,5;,
  4;36,37,15,7;,
  4;24,25,17,16;,
  4;18,17,25,26;,
  4;19,18,26,1;,
  4;35,35,27,20;,
  4;27,28,21,20;,
  4;28,29,22,21;,
  4;23,22,29,30;,
  4;30,31,31,23;,
  4;0,8,25,24;,
  4;26,25,8,12;,
  4;1,26,12,1;,
  4;35,36,7,27;,
  4;54,28,27,7;,
  4;55,29,28,54;,
  4;30,29,55,56;,
  4;56,31,31,30;,
  4;40,41,32,52;,
  4;33,32,41,42;,
  4;34,33,42,43;,
  4;45,44,35,57;,
  4;44,44,35,35;,
  4;44,45,36,35;,
  4;58,46,37,59;,
  4;46,47,38,37;,
  4;39,38,47,48;,
  4;31,31,49,49;,
  4;31,31,49,49;,
  4;31,31,49,49;,
  4;60,61,41,40;,
  4;42,41,61,62;,
  4;43,42,62,63;,
  4;19,44,45,3;,
  4;1,44,44,19;,
  4;1,45,44,1;,
  4;64,46,58,65;,
  4;66,47,46,64;,
  4;48,47,66,67;,
  4;49,49,0,24;,
  4;49,49,24,16;,
  4;49,49,16,2;;
 }
 MeshTextureCoords {
  64;
  0.168565;0.495815;,
  0.167202;0.480834;,
  0.132292;0.480436;,
  0.132325;0.493867;,
  0.202602;0.495731;,
  0.198352;0.499014;,
  0.244344;0.499815;,
  0.238832;0.493606;,
  0.238799;0.480176;,
  0.203893;0.480744;,
  0.244178;0.431846;,
  0.198190;0.433077;,
  0.203013;0.439104;,
  0.238697;0.438276;,
  0.248168;0.490081;,
  0.248138;0.478027;,
  0.244557;0.477054;,
  0.244585;0.488368;,
  0.197300;0.477170;,
  0.193723;0.478160;,
  0.193744;0.486410;,
  0.197327;0.488484;,
  0.197218;0.443612;,
  0.193651;0.448563;,
  0.173777;0.477227;,
  0.173804;0.488541;,
  0.273886;0.493520;,
  0.273853;0.480090;,
  0.268080;0.476996;,
  0.268108;0.488311;,
  0.248049;0.441742;,
  0.244475;0.443496;,
  0.173729;0.457665;,
  0.167138;0.454891;,
  0.132231;0.455351;,
  0.273791;0.455004;,
  0.268032;0.457434;,
  0.248086;0.456700;,
  0.244509;0.457492;,
  0.238738;0.455090;,
  0.203829;0.454801;,
  0.197252;0.457607;,
  0.193671;0.456833;,
  0.173695;0.443670;,
  0.167877;0.439190;,
  0.132190;0.438537;,
  0.273750;0.438190;,
  0.267998;0.443439;,
  0.179046;0.486894;,
  0.175189;0.500398;,
  0.102784;0.500161;,
  0.056792;0.499361;,
  0.124583;0.501297;,
  0.266144;0.500951;,
  0.262867;0.490545;,
  0.262837;0.478285;,
  0.262784;0.456440;,
  0.262747;0.441209;,
  0.265972;0.430603;,
  0.124411;0.430950;,
  0.175021;0.431807;,
  0.178952;0.448201;,
  0.178972;0.456645;,
  0.179026;0.478490;;
 }
}
