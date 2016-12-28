// Upgrade NOTE: replaced '_Object2World' with 'unity_ObjectToWorld'

Shader "Custom/PlayerShader" {
	Properties {
		_Color ("Color", Color) = (1,1,1,1)
		_MainTex ("Albedo (RGB)", 2D) = "white" {}
		_Cutoff("Cutoff",Range(0,1)) = 0
		_BumpMap("Normal Map", 2D) = "bump" {}
		_MetallicGlossMap("Metallic", 2D) = "white" {}
		_OutlineColor("Outline Color", Color) = (0,0,0,1)
		_OutlineSmoothness("Outline Smoothness", Color) = (0,0,0,0)
		_OutlineEmission("Outline Emission", Color) = (0,0,0,0)
		_OutlineSize("Outline width", Range(-1, 1)) = 1
		_AlphaBlend("Alpha Blend",Range(0,1)) = 1
	}
	SubShader {
		Tags {}
		LOD 200
		//-------------------------------------------------------------------
		//This makes an outline
		//
		Pass{
			Name "Outline"
			Tags{ "LightMode" = "Deferred" }
			Blend SrcAlpha OneMinusSrcAlpha
			Cull front
			ZWrite Off
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
				uniform float4 _OutlineColor;
				float _OutlineSize;
				float4 _OutlineSmoothness;
				float4	_OutlineEmission;
				float _AlphaBlend;
				float _DotClip;
				#pragma vertex vert
				#pragma fragment frag
				#include "UnityStandardCore.cginc"
				#include "UnityCG.cginc"

				v2f vert(appdata i)	{
					v2f o;

					float4 wPos = mul(unity_ObjectToWorld, i.vertex);
					float4 wNor = mul(unity_ObjectToWorld, float4(i.normal.xyz, 0.0f));
					o.pos = mul(UNITY_MATRIX_VP, wPos + wNor * _OutlineSize);
					o.normal = i.normal;
					o.viewDir = WorldSpaceViewDir(i.vertex);
					return o;
				}
				void frag(
					v2f i,
					out half4 outDiffuse : SV_Target0,            // RT0: diffuse color (rgb), occlusion (a)
					out half4 outSpecSmoothness : SV_Target1,    // RT1: spec color (rgb), smoothness (a)
					out half4 outNormal : SV_Target2,            // RT2: normal (rgb), --unused, very low precision-- (a)
					out half4 outEmission : SV_Target3            // RT3: emission (rgb), --unused-- (a)
				) {
					clip(-0.01 + _AlphaBlend);
					outDiffuse = _OutlineColor * _AlphaBlend;
					outSpecSmoothness = _OutlineSmoothness;
					outNormal = half4(i.normal.x, i.normal.y, i.normal.z, 0.33); // this normal might cause some weirdness if you use any post processes that use normals
					outEmission = _OutlineEmission;
					#ifndef UNITY_HDR_ON
						outEmission.rgb = exp2(-outEmission.rgb);
					#endif
				}
			ENDCG
		}

		CGPROGRAM
			// Physically based Standard lighting model, and enable shadows on all light types
			#pragma surface surf Standard fullforwardshadows

			// Use shader model 3.0 target, to get nicer looking lighting
			#pragma target 3.0
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
