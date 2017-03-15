Shader "Custom/SurfaceToonish" {
    Properties {
        _Color ("Color", Color) = (1,1,1,1)
        _MainTex ("Albedo (RGB)", 2D) = "white" {}
        _AlphaCutoff("Cutoff", Range(0,1)) = 0.5
        _Angle("Dot", Range(0,1)) = 0.3
        _Bright("Bright", Float) = 1
        _Dark("Dark", Range(0,1)) = 0
        _OutlineWidth("Outline Width", Range(0,1)) = 0.1
        _OutlineColor("Outline Color", Color) = (0,0,0,1)
    }
    SubShader {
        Tags {"RenderType"="Opaque"}
        LOD 200
        //Outline pass
        Pass{
            Cull Front
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            //will only need vertex position
            struct v2f {
                float4 vertex : SV_POSITION;
            };

            float _OutlineWidth;
            float4 _OutlineColor;
            v2f vert(appdata_base v) {
                v2f o;
                //set the vertex at world position + normal multiplied with width
                o.vertex = UnityObjectToClipPos(v.vertex + v.normal * _OutlineWidth);
                return o;
            }

            fixed4 frag(v2f i) : SV_Target{
                return _OutlineColor;
            }
            ENDCG
        }

        CGPROGRAM
        // Physically based Standard lighting model, and enable shadows on all light types
        #pragma surface surf Toonish
        //#pragma surface surf Lambert


        float _Angle;
        float _Bright;
        float _Dark;
        float _AlphaCutoff;
        half4 LightingToonish(SurfaceOutput s, half3 lightDir, half atten) {
            clip(s.Alpha - _AlphaCutoff);
            half NdotL = dot(s.Normal, lightDir);
            if (NdotL > _Angle) {
                NdotL = _Bright;
            } else {
                NdotL *= _Dark;
            }
            half4 c;
            c.rgb = s.Albedo * _LightColor0.rgb  * atten * NdotL;
            c.a = s.Alpha;
            return c;
        }
        // Use shader model 3.0 target, to get nicer looking lighting
        #pragma target 3.0

        sampler2D _MainTex;

        struct Input {
            float2 uv_MainTex;
        };

        half _Glossiness;
        half _Metallic;
        fixed4 _Color;


        void surf (Input IN, inout SurfaceOutput o) {
            // Albedo comes from a texture tinted by color
            fixed4 c = tex2D (_MainTex, IN.uv_MainTex) * _Color;
            clip(c.a - _AlphaCutoff);
            o.Albedo = c.rgb;
            // Metallic and smoothness come from slider variables
            //o.Metallic = _Metallic;
            //o.Smoothness = _Glossiness;
            o.Alpha = c.a;
        }


        ENDCG
    }
    FallBack "Diffuse"
}
