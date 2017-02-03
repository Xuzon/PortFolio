Shader "Custom/Silhouette" {
    Properties {
        _Color ("Color", Color) = (1,1,1,1)
        _MainTex ("Albedo (RGB)", 2D) = "white" {}
        _DotProduct("Rim Effect", Range(-1,1)) = 0.25
    }
    SubShader {
        Tags {"Queue" = "Transparent" "IgnoreProjector" = "True" "RenderType"="Transparent" }
        LOD 200
        Cull Off
        CGPROGRAM
        //Hologram so no need of PBR, transparency, and don't do any shadow
        #pragma surface surf Lambert alpha:fade nolighting

        // Use shader model 3.0 target, to get nicer looking lighting
        #pragma target 3.0

        sampler2D _MainTex;

        struct Input {
            float2 uv_MainTex;
            float3 worldNormal;
            float3 viewDir;
        };

        half _Glossiness;
        half _Metallic;
        fixed4 _Color;
        float _DotProduct;

        void surf (Input IN, inout SurfaceOutput o) {
            // Albedo comes from a texture tinted by color
            fixed4 c = tex2D (_MainTex, IN.uv_MainTex) * _Color;
            o.Albedo = c.rgb;
            float border = 1 - (abs(dot(IN.viewDir, IN.worldNormal)));
            float alpha = (border * (1 - _DotProduct) + _DotProduct);
            o.Alpha = c.a * alpha;
        }
        ENDCG
    }
    FallBack "Diffuse"
}
