Shader "Custom/Toon" {
	Properties{
		_Color("Color", Color) = (1,1,1,1)
		_MainTex("Albedo (RGB)", 2D) = "white" {}
		_Cutoff("Cutoff",Range(0,1)) = 0
		_BumpMap("Normal Map", 2D) = "bump" {}
		_MetallicGlossMap("Metallic", 2D) = "white" {}
		_Ramp("Ramp",2D) = "white"{}
		_OutlineColor("Outline Color", Color) = (0,0,0,1)
		_OutlineSize("Outline width", Range(-1, 1)) = 1
		_DotClip("Dot Clip", Range(-1,1)) = 0
	}
		SubShader{
			Tags{}
			LOD 200
			//-------------------------------------------------------------------
			//This makes an outline
			//
			Pass{
				Name "Outline"
				Tags{}
				Blend SrcAlpha OneMinusSrcAlpha
				Cull front
				CGPROGRAM
					struct appdata {
						float4 vertex :	POSITION;
						float3 normal : NORMAL;
					};

					struct v2f {
						float4 pos : POSITION;
						float3 normal : NORMAL;
						float3 viewDir : VIEWDIR;
					};
					float4 _OutlineColor;
					float _OutlineSize;
					#pragma vertex vert
					#pragma fragment frag
					#include "UnityStandardCore.cginc"
					#include "UnityCG.cginc"

					v2f vert(appdata i) {
						v2f o;
						float4 wPos = mul(unity_ObjectToWorld, i.vertex);
						float4 wNor = mul(unity_ObjectToWorld, float4(i.normal.xyz, 0.0f));
						o.pos = mul(UNITY_MATRIX_VP, wPos + wNor * _OutlineSize);
						o.normal = i.normal;
						o.viewDir = WorldSpaceViewDir(i.vertex);
						return o;
					}
					half _DotClip;
					half4 frag(v2f i) : COLOR{
						half nDot = dot(i.viewDir,i.normal);
						clip(_DotClip - nDot);
						half4 c;
						c = _OutlineColor;
						return c;
					}
				ENDCG
			}

			CGPROGRAM

				#pragma surface surf Toonish
				#include "UnityCG.cginc"
				#include "UnityPBSLighting.cginc"
				sampler2D _Ramp;
				half4 LightingToonish (SurfaceOutputStandard s, half3 lightDir, half atten) {
					half NdotL = dot(s.Normal, lightDir);
					half diff = NdotL * 0.5 + 0.5;
					half3 ramp = tex2D(_Ramp, float2(diff,0)).rgb;
					half4 c;
					c.rgb = s.Albedo * _LightColor0 * ramp * atten;
					c.a = s.Alpha;
					return c;
				}

				sampler2D _MainTex;
				sampler2D _BumpMap;
				sampler2D _MetallicGlossMap;
				float _Cutoff;
				struct Input {
					float2 uv_MainTex;
					float2 uv_BumpMap;
				};
				fixed4 _Color;

				void surf(Input IN, inout SurfaceOutputStandard o) {
					// Albedo comes from a texture tinted by color
					fixed4 c = tex2D(_MainTex, IN.uv_MainTex) * _Color;
					o.Albedo = c.rgb;
					o.Normal = UnpackNormal(tex2D(_BumpMap, IN.uv_BumpMap));
					fixed4 m = tex2D(_MetallicGlossMap, IN.uv_MainTex);
					// Metallic and smoothness come from slider variables
					o.Metallic = m.r;
					o.Smoothness = m.a;
					o.Alpha = c.a;
					clip(c.a - _Cutoff);
				}
			ENDCG
		}
	FallBack "Diffuse"
}
