// Upgrade NOTE: replaced '_Object2World' with 'unity_ObjectToWorld'

Shader "Custom/TerrainMat" {
    Properties {
        [Header(Color Texture)]
        _Color ("Base Color", Color) = (1,1,1,1)
		_Tiling("Texture tiling",Vector) = (50,50,50,1)
        [HideInInspector]
        _MainTex("Main Texture",2DArray) = "white" {}

        [Header(Blending and noise)]
		_BlendTex("BlendTexture",2D) = "white" {}
		_NoiseRamp("Noise Ramp",2D) = "white"{}


        [Header(Triplanar)]
        _TriPlanarStrength("TriPlanar strength", Range(0,1)) = 0.5
        _FrontColor("Front color", Color) = (1,0,0,1)
        _LateralColor("Lateral color", Color) = (0,1,0,1)
        _UpColor("Up color", Color) = (0,0,1,1)
    }

    CGINCLUDE
        #include "UnityCG.cginc"
        uniform float _VoxelSize;
        uniform float _TerrainWidth;
        uniform float _TerrainLength;
		uniform float _TerrainHeight;
        uniform sampler2D _FogOfWar;
    ENDCG

    SubShader {

        Tags { "RenderType"="Opaque" }
        LOD 200

        CGPROGRAM
        //#pragma surface surf Lambert fullforwardshadows vertex:vert addshadow
        //surface method = surf, Lighting method = Flat, fullforwardshadows, vertex method = vert, project shadow
        #pragma surface surf Flat fullforwardshadows vertex:vert addshadow
        #pragma target 4.0
		#pragma require 2darray


        struct Input 
        {
            float2 uv_MainTex;
            float3 localPos;
            float3 worldPos;
			float3 worldNormal;
        };

        half4 LightingFlat (SurfaceOutput s, half3 lightDir, half atten) 
        {
            half4 c;
            c.rgb = s.Albedo * atten;
            c.a = s.Alpha;
            return c;
        }


        void vert (inout appdata_full v, out Input o) 
        {
            float4 vPos = mul(unity_ObjectToWorld,v.vertex);
            UNITY_INITIALIZE_OUTPUT(Input,o);
            o.localPos = v.vertex.xyz;
        }

        fixed4 _Color;
        float _TriPlanarStrength;
        fixed4 _FrontColor;
        fixed4 _LateralColor;
        fixed4 _UpColor;
		UNITY_DECLARE_TEX2DARRAY(_MainTex);
        uniform sampler2D _BlendTex;
        float4 _BlendTex_ST;
        uniform sampler2D _NoiseTex;
        uniform sampler2D _RandomTex;
        sampler2D _NoiseRamp;
		float4 _Tiling;

        UNITY_INSTANCING_BUFFER_START(Props)
        // put more per-instance properties here
        UNITY_INSTANCING_BUFFER_END(Props)

        void surf (Input IN, inout SurfaceOutput o) 
        {
			float4 c = float4(0,0,0,1); 
            float4 toBlend = float4(1, 1, 1, 1);

			//triplanar texturing
			// up/bottom (+-y) faces
            float2 worldPos = float2(IN.worldPos.x / _TerrainWidth,IN.worldPos.z/_TerrainLength);
			float3 upUV = float3(worldPos * _Tiling.y,IN.uv_MainTex.x);
            float upDot = abs(dot(IN.worldNormal,float3(0,1,0)));
			c += UNITY_SAMPLE_TEX2DARRAY(_MainTex, upUV) * upDot;
            //check I have to blend with other terrain texture
            if(IN.uv_MainTex.y > 0)
            {
                //the other terrain value will be the standard + 1, so, we need to substract
                float3 toBlendUV = float3(worldPos * _Tiling.y, IN.uv_MainTex.y - 1);
                float4 toBlend = UNITY_SAMPLE_TEX2DARRAY(_MainTex, toBlendUV) * upDot;
                //get lerp value of blending in the blend texture
                float blendValue = tex2D(_BlendTex, upUV * _BlendTex_ST.xy);
                c = lerp(c,toBlend,blendValue);
            }

			// right/left +-x faces
			float2 zWorldPos = float2(IN.worldPos.z / _TerrainLength,IN.worldPos.y/_TerrainLength);
			float3 zUV = float3(zWorldPos * _Tiling.x,IN.uv_MainTex.x);
            float lateralDot = abs(dot(IN.worldNormal,float3(1,0,0)));
			c += UNITY_SAMPLE_TEX2DARRAY(_MainTex, zUV) * lateralDot;

			// front/back +-z faces
            //float2 xWorldPos = float2(IN.worldPos.x / _TerrainWidth,IN.worldPos.y/_TerrainHeight);
            float2 xWorldPos = float2(IN.worldPos.x / _TerrainWidth,IN.worldPos.y / _TerrainWidth);
			float3 xUV = float3(xWorldPos * _Tiling.z,IN.uv_MainTex.x);
            float frontDot = abs(dot(IN.worldNormal,float3(0,0,1)));
			c += UNITY_SAMPLE_TEX2DARRAY(_MainTex, xUV) * frontDot;

            //tint color
            c *= _Color;
            //apply triplanar tints
            float4 triplanarTint = upDot * _UpColor + frontDot * _FrontColor + lateralDot * _LateralColor;
            c = lerp(c, c*triplanarTint, _TriPlanarStrength);
            //c = lerp(c, triplanarTint, _TriPlanarStrength);
			//apply noise
            float noise = tex2D(_NoiseTex,worldPos).x;
			//apply random texture
            float random = tex2D(_RandomTex,worldPos).x;
            random = tex2D(_NoiseRamp,float2(random,0)).x;
            noise = tex2D(_NoiseRamp,float2(noise,0)).x * random;
            c *= noise;
			//apply fog of war
            float4 fogOfWar = tex2D(_FogOfWar,worldPos);
            o.Albedo = c * fogOfWar.xyz;
            o.Alpha = c.a;
        }
        ENDCG
    }
    FallBack "Diffuse"
}
