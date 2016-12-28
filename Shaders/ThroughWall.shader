Shader "Custom/ThroughWall" {
	Properties {
		_Color ("Color", Color) = (1,1,1,1)
	}
	SubShader {
		Tags { "RenderType"="Opaque" }
		LOD 200
		

		//-------------------------------------------------------------------
		//This makes an outline
		//
		Pass{
		Name "Outline"
		Tags{ "LightMode" = "Deferred" }
		Blend SrcAlpha OneMinusSrcAlpha
		Cull front
		ZTest Always
		ZWrite On
		CGPROGRAM
		struct appdata {
		float4 vertex :	POSITION;
		float3 normal : NORMAL;
	};

	struct v2f {
		float4 pos : POSITION;
		float3 normal : NORMAL;
	};
	uniform float4 _OutlineColor;
	float _OutlineSize;
	float4 _OutlineSmoothness;
	float4	_OutlineEmission;
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
		return o;
	}
	void frag(
		v2f i,
		out half4 outDiffuse : SV_Target0,            // RT0: diffuse color (rgb), occlusion (a)
		out half4 outSpecSmoothness : SV_Target1,    // RT1: spec color (rgb), smoothness (a)
		out half4 outNormal : SV_Target2,            // RT2: normal (rgb), --unused, very low precision-- (a)
		out half4 outEmission : SV_Target3            // RT3: emission (rgb), --unused-- (a)
	) {
		outDiffuse = _OutlineColor;
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

		struct Input {
			float3 worldNormal;
			float3 viewDir;
		};

		half _Glossiness;
		half _Metallic;
		fixed4 _Color;

		void surf (Input IN, inout SurfaceOutputStandard o) {
			// Albedo comes from a texture tinted by color
			fixed4 c = _Color;
			half rim = 1.0 - saturate(dot(normalize(IN.viewDir), IN.worldNormal));
			c = c * rim;
			o.Albedo = c.rgb;
			// Metallic and smoothness come from slider variables
			o.Metallic = _Metallic;
			o.Smoothness = _Glossiness;
			o.Alpha = c.a;
		}
		ENDCG
	}
	FallBack "Diffuse"
}
