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
 22;
 1.16157;0.15983;-4.72594;,
 -4.48700;0.04975;-2.11125;,
 -1.16108;-0.10541;5.06752;,
 4.48750;0.00471;2.45293;,
 7.83337;-11.48241;-2.35817;,
 13.28487;-11.48241;-5.35575;,
 4.48750;0.00471;2.45293;,
 -1.16108;-0.10541;5.06752;,
 4.01575;-11.48241;-9.29217;,
 7.83337;-11.48241;-2.35817;,
 -1.16108;-0.10541;5.06752;,
 9.46736;-11.48241;-12.28968;,
 4.01575;-11.48241;-9.29217;,
 -4.48700;0.04975;-2.11125;,
 1.16157;0.15983;-4.72594;,
 13.28487;-11.48241;-5.35575;,
 9.46736;-11.48241;-12.28968;,
 4.48750;0.00471;2.45293;,
 1.16157;0.15983;-4.72594;,
 -4.48700;0.04975;-2.11125;,
 -1.16108;-0.10541;5.06752;,
 4.48750;0.00471;2.45293;;
 
 8;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,1;,
 4;11,12,13,14;,
 4;15,16,0,17;,
 4;18,19,20,21;,
 4;16,12,4,15;,
 4;5,9,8,11;;
 
 MeshMaterialList {
  8;
  8;
  4,
  5,
  5,
  5,
  5,
  4,
  0,
  0;;
  Material {
   0.643200;0.800000;0.068800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.772000;0.536800;0.690400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.712000;0.624000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.091200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.492800;0.749600;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.489600;0.489600;0.489600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
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
  9;
  0.349144;0.923123;-0.161068;,
  -0.007811;0.999651;0.025222;,
  0.440680;-0.234547;0.866481;,
  -0.349928;-0.920648;0.173085;,
  -0.440911;0.231977;-0.867055;,
  -0.644321;-0.695184;0.318700;,
  0.352007;0.919655;-0.174142;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;;
  8;
  4;0,1,1,0;,
  4;2,2,2,2;,
  4;3,3,5,5;,
  4;4,4,4,4;,
  4;6,6,0,0;,
  4;1,1,1,1;,
  4;6,7,7,6;,
  4;8,3,3,8;;
 }
 MeshTextureCoords {
  22;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.441950;,
  0.000000;0.435000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.448880;,
  0.000000;0.441950;,
  0.000000;0.000000;,
  1.000000;0.441930;,
  0.000000;0.448880;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.435000;,
  0.000000;0.441930;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;;
 }
}
