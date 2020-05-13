// Upgrade NOTE: replaced '_Object2World' with 'unity_ObjectToWorld'

// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/GrassShader"{
    Properties{
        _Color("Color",Color) = (1,1,1,1)
        _BottomColor("Bottom Color",Color) = (0.3,0.7,0,1)
        _TopColor("Top Colot",Color) = (0.4,0.7,0.7,1)
        _Ramp("Ramp texture",2D) = "white"{}
        _Steps("Grass Vertex Steps", Int) = 8
        _Length("Grass Length", Float) = 1
        _Width("Grass Width",Range(0,1)) = 0.1
        _Gravity("Gravity",Float) = 1
        _WindForce("Wind Force", Float) = 1
        _WaveTime("Wave Time", Float) = 1
        _NoiseStrength("Noise strength",Range(0,1)) = 0.4
    }
    SubShader	{
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass{
            //Tags{ "LightMode" = "ForwardBase" }
            CGPROGRAM

            #pragma target 5.0
            #pragma vertex vert
            #pragma fragment frag
            #pragma geometry Strand
            #include "UnityCG.cginc" // for UnityObjectToWorldNormal
            #include "UnityLightingCommon.cginc" // for _LightColor0
            #include "SimplexNoise3D.hlsl"


            struct v2g {
                float4 pos : POSITION;
                float3 nor : NORMAL;
            };


            struct g2f {
                float4 pos : SV_POSITION;
                float3 normal : NORMAL;
                float4 vColor : TEXCOORD0;
            };

            v2g vert (appdata_full v) {
                v2g o;
                o.pos = v.vertex;
                o.nor = v.normal;
                return o;
            }

            //Designer modifying values
            float _Steps;
            float _Length;
            float _Width;
            float _Gravity;
            float _WindForce;
            float _WaveTime;
            float _NoiseStrength;
            float4 _BottomColor;
            float4 _TopColor;

            //Calculate the normal of the face and fill the buffer with the given positions
            void FillTri(float4 pos0, float4 pos1, float4 pos2, float4 pos3, inout g2f buffer[4]) {
                float3 normal = UnityObjectToWorldNormal(normalize(cross(pos1 - pos0, pos2 - pos0)));
                buffer[0].pos = UnityObjectToClipPos(pos0);
                buffer[0].normal = normal;
                buffer[1].pos = UnityObjectToClipPos(pos1);
                buffer[1].normal = normal;
                buffer[2].pos = UnityObjectToClipPos(pos2);
                buffer[2].normal = normal;
                buffer[3].pos = UnityObjectToClipPos(pos3);
                buffer[3].normal = normal;
            }
            //Append the buffer to the stream
            void AppendTri(g2f tris[4], inout TriangleStream<g2f> stream) {
                for (int i = 0; i < 4; i++) {
                    stream.Append(tris[i]);
                }
            }

            //Create a strand of grass giving 3 base positions, normal, tangent, wind and length
            void CreateStrand(float4 p1_1, float4 p2_1, float4 p3_1, float3 sourceNormal, float4 tangent, float wind, float length, inout TriangleStream<g2f> stream) {
                //face buffer
                g2f triOutput[4];
                //creation of the strand
                for (int i = 0; i <= _Steps; i++) {
                    //get current lerping
                    float t = (float)(i) / _Steps;
                    for (int j = 0; j < 4; j++) {
                        triOutput[j].vColor = lerp(_BottomColor, _TopColor, t);
                    }
                    //Get the grow direction by lerping the normal of the source triangle
                    //to the tangent of the triangle (make it look to top)
                    //multiplying the lerping with the gravity and by wind
                    float4 growDirection = normalize(lerp(float4(sourceNormal, 0), tangent, t * _Gravity * wind));
                    //calculate the length of this face
                    float faceLength = t * length;
                    //add to the last point the grow direction in the length
                    float4 p1_2 = p1_1 + growDirection * faceLength;
                    float4 p2_2 = p2_1 + growDirection * faceLength;
                    float4 p3_2 = p3_1 + growDirection * faceLength;
                    //fill the faces of the grass
                    FillTri(p1_1, p2_1, p1_2, p2_2, triOutput);
                    AppendTri(triOutput, stream);
                    FillTri(p2_1, p3_1, p2_2, p3_2, triOutput);
                    AppendTri(triOutput, stream);
                    FillTri(p3_1, p1_1, p3_2, p1_2, triOutput);
                    AppendTri(triOutput, stream);
                    //make the new points the last points
                    p1_1 = p1_2;
                    p2_1 = p2_2;
                    p3_1 = p3_2;
                }
            }

            [maxvertexcount(90)]
            void Strand(triangle v2g i[3], inout TriangleStream<g2f> stream){
                //repack positions to have clean variables
                float4 p1 = i[0].pos;
                float4 p2 = i[1].pos;
                float4 p3 = i[2].pos;
                //Compute center of the triangle
                float4 P = (p1 + p2 + p3) / 3.0f;

                //Compute normal of the triangle
                float3 sourceNormal = (i[0].nor + i[1].nor + i[2].nor) / 3.0f;

                //Get the start points of the strand using the width
                float4 p1_1 = lerp(P, p1, _Width);
                float4 p2_1 = lerp(P, p2, _Width);
                float4 p3_1 = lerp(P, p3, _Width);
                //get the tangent of the source triangle
                float4 tangent = float4(normalize((p2 - p1).xyz), 0.0f);
                //get noise for randomness
                float noise = cnoise(mul(unity_ObjectToWorld,P.xyz));
                float iNoise = cnoise(mul(unity_ObjectToWorld,-P.xyz));
                //move a little bit the tangent to give randomness to grow direction
                tangent += noise * (p3 - p1) + iNoise * (p2-p1);
                tangent = normalize(tangent);
                //introduce randomness in length
                float length = _Length;
                length *= (abs(noise) / _NoiseStrength);
                //randomize a little bit the position to feel more natural and
                //less artificial
                p1_1 -= tangent * noise;
                p2_1 -= tangent * noise;
                p3_1 -= tangent * noise;
                //wind variables
                float c = cos(_Time.w * _WaveTime * abs(noise));
                c *= _WindForce;
                //create the grass strand
                CreateStrand(p1_1, p2_1, p3_1, sourceNormal, tangent, c, length, stream);
                stream.RestartStrip();
                
                /*p1_1 = lerp(p1, P, _Width);
                p2_1 = lerp(p1, p2, _Width);
                p3_1 = lerp(p1, p3, _Width);
                CreateStrand(p1_1, p2_1, p3_1, sourceNormal, tangent, c, length, stream);
                stream.RestartStrip();
                p1_1 = lerp(p2, P, _Width);
                p2_1 = lerp(p2, p3, _Width);
                p3_1 = lerp(p2, p1, _Width);
                CreateStrand(p1_1, p2_1, p3_1, sourceNormal, tangent, c, length, stream);
                stream.RestartStrip();
                p1_1 = lerp(p3, P, _Width);
                p2_1 = lerp(p3, p1, _Width);
                p3_1 = lerp(p3, p2, _Width);
                CreateStrand(p1_1, p2_1, p3_1, sourceNormal, tangent, c, length, stream);
                stream.RestartStrip();*/
            }

           
            float4 _Color;
            sampler2D _Ramp;
            fixed4 frag (g2f i) : SV_Target{
                //fixed4 col = _Color;
                fixed4 col = i.vColor;
                //compute dot product
                float nl = max(0, dot(i.normal, _WorldSpaceLightPos0.xyz));
                //make it go from 0 to 1
                float diff = nl * 0.5f + 0.5f;
                //get the ramp color of the texture and dot product
                fixed4 rampColor = tex2D(_Ramp, float2(diff, 0));
                //multiply the color with the light color and ramp color
                col *= _LightColor0 * rampColor;;
                // apply fog
                //UNITY_APPLY_FOG(i.fogCoord, col);
                return col;
            }
            ENDCG
        }
    }
}
