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
 138;
 -5.91344;0.09854;-3.47536;,
 -7.17114;-0.34560;-3.18796;,
 -6.87974;-1.19802;-2.93630;,
 -5.62442;-0.74114;-3.56654;,
 -7.68864;-2.38872;-3.95280;,
 -5.38586;-1.04056;-3.86672;,
 -7.57362;-2.71184;-4.65960;,
 -5.26682;-1.38478;-4.00560;,
 -7.69844;-2.33732;-5.33732;,
 -5.38736;-1.03260;-4.08112;,
 -7.98974;-1.48526;-5.58898;,
 -5.62768;-0.72414;-4.02450;,
 -8.27710;-0.65416;-5.26716;,
 -5.91494;0.10650;-3.68976;,
 -8.39204;-0.33122;-4.56036;,
 -6.03368;0.44922;-3.51682;,
 -9.32192;-1.06778;-3.89338;,
 -9.03050;-1.92018;-3.64170;,
 -8.74328;-2.75092;-3.96352;,
 -8.62822;-3.07410;-4.67032;,
 -8.75308;-2.69956;-5.34804;,
 -9.04432;-1.84752;-5.59972;,
 -9.33170;-1.01640;-5.27788;,
 -9.44666;-0.69348;-4.57110;,
 -13.12022;-2.14860;-2.44090;,
 -13.14378;-3.41704;-2.38640;,
 -13.05766;-4.31644;-2.73566;,
 -13.42876;-4.80360;-3.50822;,
 -14.03940;-4.59410;-4.25184;,
 -14.53214;-3.80970;-4.53080;,
 -14.61804;-2.91102;-4.18168;,
 -14.24702;-2.42354;-3.40898;,
 -13.69684;-2.74964;0.43822;,
 -13.20620;-3.52312;0.44234;,
 -13.11824;-4.43272;0.36806;,
 -13.48468;-4.94454;0.25896;,
 -14.09058;-4.75960;0.17890;,
 -14.58140;-3.98546;0.17482;,
 -14.66922;-3.07650;0.24906;,
 -13.63566;-2.35182;1.04846;,
 -5.62736;-0.72588;-3.97764;,
 -13.88926;-3.77854;0.96192;,
 -6.59190;3.81516;-0.41304;,
 -5.41522;3.63048;-1.28250;,
 -6.91774;3.48070;-2.44292;,
 -7.38088;3.81304;-1.02196;,
 -11.94142;3.28646;-3.65522;,
 -8.35982;3.80008;-1.03148;,
 -12.37514;3.22084;-1.59224;,
 -9.15486;3.78128;-0.43796;,
 -12.40584;2.86758;0.49028;,
 -9.46226;3.76376;0.53188;,
 -12.39622;3.16284;2.58292;,
 -9.16472;3.75430;1.50760;,
 -11.98344;3.17096;4.65508;,
 -8.37570;3.75638;2.11652;,
 -6.94800;3.39746;3.54494;,
 -7.39676;3.76928;2.12604;,
 -5.43394;3.57902;2.41820;,
 -6.60172;3.78806;1.53252;,
 -4.84916;3.61230;0.57344;,
 -6.29432;3.80564;0.56268;,
 -4.47204;3.08498;-1.97846;,
 -6.53758;3.07942;-3.57262;,
 -12.92616;2.86722;-3.63666;,
 -13.29706;3.02472;-1.63416;,
 -13.26522;2.73360;0.47994;,
 -13.31848;2.96592;2.60110;,
 -12.96782;2.75268;4.60490;,
 -6.57914;2.96492;4.66896;,
 -4.49776;3.01418;3.11512;,
 -3.69292;3.06002;0.57604;,
 -0.58682;1.67024;-1.42278;,
 -6.28992;1.66382;-4.31524;,
 -13.63852;1.34368;-3.75198;,
 -13.85504;1.45612;-1.72984;,
 -13.93668;1.47648;0.45462;,
 -13.87712;1.39538;2.63888;,
 -13.68102;1.22676;4.66072;,
 -6.33888;1.52916;5.37330;,
 -0.61704;1.58710;2.52826;,
 0.50350;1.64098;0.56178;,
 -0.36202;0.09142;-1.59852;,
 -6.19010;0.08458;-4.58742;,
 -14.27986;-0.10574;-3.83046;,
 -14.47934;0.00316;-1.75302;,
 -14.61798;0.04522;0.42658;,
 -14.50138;-0.05744;2.60642;,
 -14.32288;-0.22404;4.68332;,
 -6.24156;-0.05696;5.59972;,
 -10.11634;-0.13706;5.16032;,
 -0.39382;0.00392;2.66066;,
 0.77542;0.06062;0.54058;,
 -0.54470;-1.49714;-1.47006;,
 -6.24784;-1.50356;-4.36252;,
 -12.96460;-2.31790;-3.90120;,
 -13.08578;-2.32588;-1.75258;,
 -13.35098;-2.42046;0.38458;,
 -13.10142;-2.40246;2.52804;,
 -12.99594;-2.47134;4.67730;,
 -10.19228;-0.15876;5.05776;,
 -6.29680;-1.63822;5.32602;,
 -0.57496;-1.58030;2.48098;,
 0.54558;-1.52642;0.51450;,
 -4.39198;-2.93978;-2.06838;,
 -6.44170;-1.69864;-3.64386;,
 -11.53804;-1.59294;-3.81678;,
 -11.06490;-2.77874;-1.64194;,
 -11.24044;-2.92668;0.39164;,
 -11.07978;-2.85162;2.43222;,
 -10.82814;-2.74714;4.63010;,
 -10.19744;-0.19068;4.64646;,
 -6.49914;-3.05980;4.57902;,
 -4.41770;-3.01054;3.02518;,
 -3.61284;-2.96470;0.48610;,
 -5.29784;-2.42566;-1.39780;,
 -6.76952;-1.81244;-2.52176;,
 -11.85254;-1.74364;-3.82652;,
 -10.38682;-1.62776;-1.63898;,
 -10.42668;-1.73422;0.42096;,
 -10.40190;-1.70152;2.48410;,
 -10.23472;-1.18620;4.68546;,
 -10.17854;-0.30996;4.29610;,
 -6.82878;-2.51290;3.43182;,
 -5.31654;-2.47710;2.30290;,
 -4.73180;-2.44380;0.45814;,
 -10.16560;-1.16158;-0.49598;,
 -10.95456;-1.16364;-1.10488;,
 -11.93354;-1.17662;-1.11440;,
 -12.72856;-1.19538;-0.52090;,
 -13.03598;-1.21292;0.44894;,
 -12.73838;-1.22244;1.42468;,
 -11.94944;-1.22034;2.03360;,
 -10.97044;-1.20742;2.04312;,
 -10.17544;-1.18860;1.44960;,
 -9.86802;-1.17110;0.47976;,
 -7.88404;3.84646;0.54818;,
 -11.46914;-0.92210;0.46826;;
 
 155;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,1,0;,
 4;1,16,17,2;,
 4;2,17,18,4;,
 4;4,18,19,6;,
 4;6,19,20,8;,
 4;8,20,21,10;,
 4;10,21,22,12;,
 4;12,22,23,14;,
 4;14,23,16,1;,
 4;16,24,25,17;,
 4;17,25,26,18;,
 4;18,26,27,19;,
 4;19,27,28,20;,
 4;20,28,29,21;,
 4;21,29,30,22;,
 4;22,30,31,23;,
 4;23,31,24,16;,
 4;24,32,33,25;,
 4;25,33,34,26;,
 4;26,34,35,27;,
 4;27,35,36,28;,
 4;28,36,37,29;,
 4;29,37,38,30;,
 4;30,38,39,31;,
 4;31,39,32,24;,
 3;3,40,0;,
 3;5,40,3;,
 3;7,40,5;,
 3;9,40,7;,
 3;11,40,9;,
 3;13,40,11;,
 3;15,40,13;,
 3;0,40,15;,
 3;32,41,33;,
 3;33,41,34;,
 3;34,41,35;,
 3;35,41,36;,
 3;36,41,37;,
 3;37,41,38;,
 3;38,41,39;,
 3;39,41,32;,
 4;42,43,44,45;,
 4;45,44,46,47;,
 4;47,46,48,49;,
 4;49,48,50,51;,
 4;51,50,52,53;,
 4;53,52,54,55;,
 4;55,54,56,57;,
 4;57,56,58,59;,
 4;59,58,60,61;,
 4;61,60,43,42;,
 4;43,62,63,44;,
 4;44,63,64,46;,
 4;46,64,65,48;,
 4;48,65,66,50;,
 4;50,66,67,52;,
 4;52,67,68,54;,
 4;54,68,69,56;,
 4;56,69,70,58;,
 4;58,70,71,60;,
 4;60,71,62,43;,
 4;62,72,73,63;,
 4;63,73,74,64;,
 4;64,74,75,65;,
 4;65,75,76,66;,
 4;66,76,77,67;,
 4;67,77,78,68;,
 4;68,78,79,69;,
 4;69,79,80,70;,
 4;70,80,81,71;,
 4;71,81,72,62;,
 4;72,82,83,73;,
 4;73,83,84,74;,
 4;74,84,85,75;,
 4;75,85,86,76;,
 4;76,86,87,77;,
 4;77,87,88,78;,
 3;89,79,90;,
 3;79,78,90;,
 3;78,88,90;,
 4;79,89,91,80;,
 4;80,91,92,81;,
 4;81,92,82,72;,
 4;82,93,94,83;,
 4;83,94,95,84;,
 4;84,95,96,85;,
 4;85,96,97,86;,
 4;86,97,98,87;,
 4;87,98,99,88;,
 4;88,99,100,90;,
 4;89,101,102,91;,
 4;91,102,103,92;,
 4;92,103,93,82;,
 4;93,104,105,94;,
 4;94,105,106,95;,
 4;95,106,107,96;,
 4;96,107,108,97;,
 4;97,108,109,98;,
 4;98,109,110,99;,
 4;99,110,111,100;,
 4;101,112,113,102;,
 4;102,113,114,103;,
 4;103,114,104,93;,
 4;104,115,116,105;,
 4;105,116,117,106;,
 4;106,117,118,107;,
 4;107,118,119,108;,
 4;108,119,120,109;,
 4;109,120,121,110;,
 4;110,121,122,111;,
 4;112,123,124,113;,
 4;113,124,125,114;,
 4;114,125,115,104;,
 4;115,126,127,116;,
 4;116,127,128,117;,
 4;117,128,129,118;,
 4;118,129,130,119;,
 4;119,130,131,120;,
 4;120,131,132,121;,
 3;123,122,133;,
 3;122,121,133;,
 3;121,132,133;,
 4;123,133,134,124;,
 4;124,134,135,125;,
 4;125,135,126,115;,
 3;45,136,42;,
 3;47,136,45;,
 3;49,136,47;,
 3;51,136,49;,
 3;53,136,51;,
 3;55,136,53;,
 3;57,136,55;,
 3;59,136,57;,
 3;61,136,59;,
 3;42,136,61;,
 3;126,137,127;,
 3;127,137,128;,
 3;128,137,129;,
 3;129,137,130;,
 3;130,137,131;,
 3;131,137,132;,
 3;132,137,133;,
 3;133,137,134;,
 3;134,137,135;,
 3;135,137,126;,
 4;100,101,89,90;,
 4;111,112,101,100;,
 4;122,123,112,111;;
 
 MeshMaterialList {
  1;
  155;
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
    "data\\TEXTURE\\UV\\ninja_blue.jpg";
   }
  }
 }
 MeshNormals {
  188;
  0.058191;0.389610;0.919140;,
  0.380583;-0.270344;0.884348;,
  0.340096;-0.640175;0.688847;,
  0.272692;-0.619087;0.736457;,
  0.516184;-0.115477;-0.848657;,
  0.414869;0.312919;-0.854380;,
  0.251388;0.610526;-0.751041;,
  0.102407;0.740823;-0.663848;,
  -0.163061;0.478436;0.862850;,
  0.102624;-0.234963;0.966572;,
  0.257270;-0.674972;0.691538;,
  0.285695;-0.749324;0.597404;,
  0.378452;-0.356251;-0.854318;,
  0.217068;0.185502;-0.958369;,
  0.026885;0.640624;-0.767384;,
  -0.095590;0.819097;-0.565635;,
  -0.183355;0.618649;0.763973;,
  0.083060;-0.111361;0.990303;,
  0.291322;-0.654820;0.697383;,
  0.331597;-0.942587;-0.039670;,
  0.139943;-0.613066;-0.777539;,
  -0.096273;0.008212;-0.995321;,
  -0.289035;0.623520;-0.726417;,
  -0.332282;0.942849;-0.024983;,
  0.266167;0.195970;0.943796;,
  0.322681;-0.036642;0.945798;,
  0.597061;-0.638979;0.485000;,
  0.329980;-0.943315;-0.035635;,
  -0.307420;-0.834619;-0.457061;,
  -0.188838;-0.024088;-0.981713;,
  -0.531172;0.695760;-0.483503;,
  -0.465561;0.884838;-0.017713;,
  0.885570;0.371804;0.278439;,
  0.851986;0.189136;0.488209;,
  0.820171;-0.274476;0.501978;,
  0.808515;-0.588419;-0.008126;,
  -0.586101;-0.707548;0.394793;,
  -0.879492;-0.316935;0.355030;,
  -0.891249;0.158667;0.424854;,
  -0.772391;0.436867;0.461042;,
  0.898900;0.437378;0.026077;,
  0.326492;-0.188757;0.926161;,
  0.060229;0.996884;-0.050929;,
  0.017505;0.994285;-0.105313;,
  -0.044250;0.996062;-0.076826;,
  -0.111104;0.993809;0.000699;,
  -0.139803;0.990094;0.013030;,
  -0.111339;0.993448;0.025773;,
  -0.045944;0.993509;0.104058;,
  0.015288;0.990982;0.133123;,
  0.059022;0.995105;0.079259;,
  0.079090;0.996765;0.014279;,
  0.214351;0.962999;-0.163360;,
  0.084120;0.908687;-0.408916;,
  -0.106020;0.941388;-0.320233;,
  -0.210026;0.977460;0.021461;,
  -0.205023;0.978673;0.012811;,
  -0.209914;0.977712;0.003846;,
  -0.112327;0.931679;0.345482;,
  0.076229;0.897003;0.435402;,
  0.211156;0.958328;0.192408;,
  0.284551;0.958547;0.014801;,
  0.370610;0.866824;-0.333565;,
  0.173801;0.628497;-0.758146;,
  -0.118305;0.687200;-0.716771;,
  -0.647971;0.761321;-0.022915;,
  -0.623114;0.782090;0.008001;,
  -0.648409;0.760356;0.037743;,
  -0.389841;0.657131;0.645138;,
  0.161558;0.605506;0.779270;,
  0.367729;0.855359;0.364880;,
  0.385797;0.922424;0.017141;,
  0.593260;0.521192;-0.613516;,
  0.200701;0.308146;-0.929928;,
  -0.066868;0.212479;-0.974875;,
  -0.912756;0.400900;-0.078459;,
  -0.913104;0.407725;0.001041;,
  -0.913747;0.398278;0.080253;,
  -0.911326;0.387920;0.137854;,
  0.127309;0.251283;0.959505;,
  0.586865;0.499568;0.637198;,
  0.339621;0.940361;0.019467;,
  0.698613;0.019965;-0.715221;,
  0.193210;0.022895;-0.980890;,
  -0.080858;0.024714;-0.996419;,
  -0.994996;-0.050822;-0.086021;,
  -0.999152;-0.040741;-0.005878;,
  -0.995853;-0.052923;0.073997;,
  -0.994609;-0.064139;0.081482;,
  0.143039;-0.121834;0.982190;,
  0.690222;-0.001608;0.723595;,
  0.999882;0.013176;0.007907;,
  0.552292;-0.576323;-0.602350;,
  0.226060;-0.372330;-0.900149;,
  -0.072021;-0.080865;-0.994120;,
  -0.702365;-0.707344;-0.079673;,
  -0.600632;-0.799358;-0.016359;,
  -0.583556;-0.810906;0.043520;,
  -0.567252;-0.823090;0.027337;,
  0.073087;-0.573203;0.816147;,
  0.594625;-0.509847;0.621673;,
  0.987907;-0.154981;0.004544;,
  -0.064484;-0.979212;-0.192316;,
  -0.013014;-0.898282;-0.439226;,
  0.123694;-0.984596;-0.123571;,
  -0.036450;-0.986852;-0.157461;,
  -0.223693;-0.974488;-0.018260;,
  -0.197720;-0.979728;0.032240;,
  -0.171548;-0.984964;0.020422;,
  -0.210239;-0.844105;0.493241;,
  0.418647;-0.794606;0.439700;,
  -0.041379;-0.999027;-0.015266;,
  -0.313510;-0.949391;0.019220;,
  -0.131943;-0.983506;-0.123721;,
  0.048221;-0.979552;-0.195326;,
  0.404617;0.914330;-0.016894;,
  0.844276;-0.535870;-0.006498;,
  0.874412;-0.484413;-0.027351;,
  0.952431;-0.233268;0.196113;,
  -0.387886;-0.920375;-0.049542;,
  -0.276957;-0.934091;-0.225317;,
  -0.332402;-0.943026;-0.014552;,
  -0.170012;-0.983980;0.053663;,
  -0.076438;-0.989528;0.122438;,
  0.032419;-0.985525;0.166400;,
  0.270178;0.953360;0.134571;,
  0.201864;0.979273;0.016610;,
  0.293558;0.932811;-0.209018;,
  0.373100;0.848432;-0.375446;,
  -0.139267;-0.989649;-0.034626;,
  -0.170511;-0.973622;-0.151612;,
  -0.197205;-0.980252;-0.014738;,
  -0.013405;0.999815;0.013824;,
  0.014113;-0.999805;-0.013839;,
  -0.137801;-0.195464;0.970981;,
  -0.064342;-0.665634;0.743500;,
  0.545972;-0.275054;0.791366;,
  0.601643;0.008760;0.798717;,
  0.751518;0.032796;-0.658897;,
  0.437412;-0.626507;-0.645105;,
  -0.164831;0.569177;0.805524;,
  -0.262856;0.721761;0.640288;,
  0.195133;0.967301;0.162026;,
  0.953776;0.274713;0.121840;,
  0.980262;0.183579;0.073380;,
  0.995618;0.090360;0.024088;,
  -0.313796;-0.854285;0.414402;,
  -0.945827;-0.323889;-0.022511;,
  -0.995619;-0.092388;-0.014412;,
  0.549902;0.835193;-0.007847;,
  0.879844;0.475127;0.011345;,
  0.945725;0.324924;0.005335;,
  0.809874;0.545468;-0.215798;,
  0.796341;0.517456;0.313177;,
  0.725494;0.372420;0.578759;,
  0.879873;0.475066;0.011630;,
  0.945770;0.324787;0.005692;,
  0.945755;0.324832;0.005577;,
  0.314317;-0.407775;0.857277;,
  -0.681334;-0.155447;0.715276;,
  0.984273;-0.175927;0.016009;,
  -0.909483;0.384271;-0.158670;,
  -0.909618;0.392432;-0.136353;,
  -0.092607;0.144291;0.985192;,
  -0.993648;-0.062171;-0.093803;,
  -0.092967;-0.315523;0.944353;,
  0.983433;0.180933;0.011046;,
  -0.857986;-0.510944;-0.052873;,
  0.199904;-0.613874;0.763674;,
  0.063278;-0.880858;0.469132;,
  0.114748;-0.984997;0.128893;,
  0.151386;-0.961819;-0.228006;,
  0.740484;-0.364499;0.564646;,
  0.345733;-0.938226;-0.014169;,
  0.619782;0.718278;-0.316143;,
  0.503131;0.862071;-0.060771;,
  0.844061;-0.536147;0.010335;,
  -0.246312;-0.900450;-0.358496;,
  -0.333395;-0.907545;-0.255364;,
  0.336418;0.920089;0.200648;,
  -0.516828;-0.821526;0.240799;,
  0.948422;-0.177516;-0.262648;,
  0.013153;-0.999904;0.004368;,
  0.049128;-0.992700;-0.110154;,
  0.149210;-0.985378;0.082258;,
  0.182629;-0.983099;-0.012752;,
  0.151056;-0.982597;-0.108098;,
  -0.507860;-0.813871;0.282298;;
  155;
  4;0,8,9,1;,
  4;1,9,10,2;,
  4;2,10,11,3;,
  4;138,139,12,4;,
  4;4,12,13,5;,
  4;5,13,14,6;,
  4;6,14,15,7;,
  4;140,141,8,0;,
  4;8,16,17,9;,
  4;9,17,18,10;,
  4;10,18,19,11;,
  4;139,19,20,12;,
  4;12,20,21,13;,
  4;13,21,22,14;,
  4;14,22,23,15;,
  4;141,23,16,8;,
  4;16,24,25,17;,
  4;17,25,26,18;,
  4;18,26,27,19;,
  4;19,27,28,20;,
  4;20,28,29,21;,
  4;21,29,30,22;,
  4;22,30,31,23;,
  4;23,31,142,16;,
  4;143,32,33,144;,
  4;144,33,34,145;,
  4;26,34,35,27;,
  4;27,146,36,28;,
  4;28,36,37,147;,
  4;147,37,38,148;,
  4;30,38,39,31;,
  4;149,149,32,142;,
  3;150,40,151;,
  3;152,40,150;,
  3;138,40,152;,
  3;153,40,154;,
  3;155,40,153;,
  3;156,40,155;,
  3;157,40,156;,
  3;151,40,157;,
  3;32,41,33;,
  3;33,41,34;,
  3;34,41,158;,
  3;146,41,36;,
  3;36,159,37;,
  3;37,159,38;,
  3;38,159,39;,
  3;160,160,32;,
  4;42,52,53,43;,
  4;43,53,54,44;,
  4;44,54,55,45;,
  4;45,55,56,46;,
  4;46,56,57,47;,
  4;47,57,58,48;,
  4;48,58,59,49;,
  4;49,59,60,50;,
  4;50,60,61,51;,
  4;51,61,52,42;,
  4;52,62,63,53;,
  4;53,63,64,54;,
  4;54,64,65,55;,
  4;55,65,66,56;,
  4;56,66,67,57;,
  4;57,67,68,58;,
  4;58,68,69,59;,
  4;59,69,70,60;,
  4;60,70,71,61;,
  4;61,71,62,52;,
  4;62,72,73,63;,
  4;63,73,74,64;,
  4;161,162,75,65;,
  4;65,75,76,66;,
  4;66,76,77,67;,
  4;67,77,78,68;,
  4;68,163,79,69;,
  4;69,79,80,70;,
  4;70,80,81,71;,
  4;71,81,72,62;,
  4;72,82,83,73;,
  4;73,83,84,74;,
  4;162,164,85,75;,
  4;75,85,86,76;,
  4;76,86,87,77;,
  4;77,87,88,78;,
  3;89,79,134;,
  3;79,163,134;,
  3;163,165,134;,
  4;79,89,90,80;,
  4;80,90,91,166;,
  4;166,91,82,72;,
  4;82,92,93,83;,
  4;83,93,94,84;,
  4;164,167,95,85;,
  4;85,95,96,86;,
  4;86,96,97,87;,
  4;87,97,98,88;,
  4;165,168,135,134;,
  4;89,99,100,90;,
  4;90,100,101,91;,
  4;91,101,92,82;,
  4;92,102,103,93;,
  4;169,169,104,170;,
  4;170,104,105,171;,
  4;95,105,106,96;,
  4;96,106,107,97;,
  4;97,107,108,98;,
  4;168,172,136,135;,
  4;99,109,110,100;,
  4;100,110,111,173;,
  4;173,111,102,92;,
  4;102,112,113,103;,
  4;103,113,114,104;,
  4;174,175,115,174;,
  4;176,176,116,116;,
  4;116,116,117,117;,
  4;117,117,118,172;,
  4;172,118,137,136;,
  4;177,119,120,178;,
  4;178,120,121,111;,
  4;111,121,112,102;,
  4;112,122,123,113;,
  4;113,123,124,114;,
  4;175,179,125,115;,
  4;115,125,126,126;,
  4;126,126,127,127;,
  4;127,127,128,128;,
  3;119,180,129;,
  3;181,118,181;,
  3;182,183,129;,
  4;119,129,130,120;,
  4;120,130,131,121;,
  4;121,131,122,112;,
  3;43,132,42;,
  3;44,132,43;,
  3;45,132,44;,
  3;46,132,45;,
  3;47,132,46;,
  3;48,132,47;,
  3;49,132,48;,
  3;50,132,49;,
  3;51,132,50;,
  3;42,132,51;,
  3;122,133,123;,
  3;123,133,124;,
  3;124,133,184;,
  3;184,133,185;,
  3;185,133,186;,
  3;186,133,183;,
  3;183,133,129;,
  3;129,133,130;,
  3;130,133,131;,
  3;131,133,122;,
  4;135,99,89,134;,
  4;187,109,99,135;,
  4;180,119,109,187;;
 }
 MeshTextureCoords {
  138;
  0.150023;0.052516;,
  0.157480;0.055167;,
  0.155752;0.060254;,
  0.148309;0.057527;,
  0.160549;0.067360;,
  0.146895;0.059314;,
  0.159867;0.069288;,
  0.146189;0.061368;,
  0.160607;0.067053;,
  0.146904;0.059267;,
  0.162334;0.061968;,
  0.148328;0.057426;,
  0.164038;0.057008;,
  0.150032;0.052468;,
  0.164719;0.055081;,
  0.150736;0.050423;,
  0.170233;0.059477;,
  0.168505;0.064564;,
  0.166802;0.069521;,
  0.166119;0.071450;,
  0.166860;0.069215;,
  0.168587;0.064130;,
  0.170291;0.059170;,
  0.170972;0.057243;,
  0.192754;0.065927;,
  0.192893;0.073497;,
  0.192383;0.078864;,
  0.194583;0.081772;,
  0.198204;0.080521;,
  0.201125;0.075840;,
  0.201635;0.070477;,
  0.199435;0.067568;,
  0.196173;0.069514;,
  0.193263;0.074130;,
  0.192742;0.079558;,
  0.194915;0.082613;,
  0.198507;0.081509;,
  0.201417;0.076889;,
  0.201938;0.071464;,
  0.195810;0.067139;,
  0.148327;0.057436;,
  0.197313;0.075654;,
  0.154046;0.030336;,
  0.147069;0.031438;,
  0.155978;0.032332;,
  0.158724;0.030348;,
  0.185764;0.033491;,
  0.164528;0.030426;,
  0.188336;0.033882;,
  0.169242;0.030538;,
  0.188518;0.035991;,
  0.171065;0.030642;,
  0.188461;0.034229;,
  0.169300;0.030699;,
  0.186013;0.034180;,
  0.164622;0.030686;,
  0.156157;0.032828;,
  0.158818;0.030609;,
  0.147180;0.031745;,
  0.154104;0.030497;,
  0.143712;0.031546;,
  0.152281;0.030392;,
  0.141476;0.034693;,
  0.153724;0.034726;,
  0.191603;0.035993;,
  0.193802;0.035053;,
  0.193613;0.036790;,
  0.193929;0.035404;,
  0.191850;0.036676;,
  0.153970;0.035410;,
  0.141629;0.035116;,
  0.136857;0.034842;,
  0.118440;0.043136;,
  0.152255;0.043175;,
  0.195827;0.045085;,
  0.197111;0.044414;,
  0.197595;0.044293;,
  0.197241;0.044777;,
  0.196079;0.045783;,
  0.152546;0.043978;,
  0.118619;0.043632;,
  0.111975;0.043311;,
  0.117107;0.052559;,
  0.151663;0.052599;,
  0.199630;0.053735;,
  0.200812;0.053085;,
  0.201634;0.052834;,
  0.200943;0.053447;,
  0.199885;0.054441;,
  0.151968;0.053444;,
  0.174943;0.053922;,
  0.117296;0.053081;,
  0.110363;0.052742;,
  0.118190;0.062039;,
  0.152006;0.062077;,
  0.191831;0.066937;,
  0.192550;0.066985;,
  0.194122;0.067549;,
  0.192642;0.067442;,
  0.192017;0.067853;,
  0.175393;0.054052;,
  0.152296;0.062881;,
  0.118370;0.062535;,
  0.111726;0.062214;,
  0.141002;0.070648;,
  0.153155;0.063241;,
  0.183373;0.062611;,
  0.180567;0.069687;,
  0.181608;0.070570;,
  0.180655;0.070122;,
  0.179163;0.069499;,
  0.175424;0.054242;,
  0.153496;0.071365;,
  0.141154;0.071071;,
  0.136382;0.070797;,
  0.146373;0.067580;,
  0.155099;0.063921;,
  0.185237;0.063510;,
  0.176547;0.062818;,
  0.176783;0.063454;,
  0.176636;0.063259;,
  0.175645;0.060183;,
  0.175312;0.054954;,
  0.155450;0.068101;,
  0.146484;0.067887;,
  0.143017;0.067689;,
  0.175235;0.060036;,
  0.179913;0.060049;,
  0.185718;0.060126;,
  0.190431;0.060238;,
  0.192254;0.060343;,
  0.190490;0.060399;,
  0.185812;0.060387;,
  0.180007;0.060310;,
  0.175293;0.060198;,
  0.173471;0.060093;,
  0.161707;0.030149;,
  0.182964;0.058607;;
 }
}
