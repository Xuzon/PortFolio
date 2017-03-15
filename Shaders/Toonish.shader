Shader "Custom/Toonish"{
    Properties{
        _MainTex ("Texture", 2D) = "white" {}
        _AlphaCutoff("Cutoff", Range(0,1)) = 0.5
        _Angle("Dot", Range(0,1)) = 0.3
        _Bright("Bright", Float) = 1
        _Dark("Dark", Range(0,1)) = 0
        _OutlineWidth("Outline Width", Range(0,1)) = 0.1
        _OutlineColor("Outline Color", Color) = (0,0,0,1)
    }
    SubShader{
        Tags { "RenderType"="Opaque" }
        LOD 100
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

        // shadow caster rendering pass, implemented manually
        // using macros from UnityCG.cginc
        Pass{
            Tags{ "LightMode" = "ShadowCaster" }

            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #pragma multi_compile_shadowcaster
            #include "UnityCG.cginc"

            struct v2f {
                V2F_SHADOW_CASTER;
                float2 uv : TEXCOORD0;
            };
            sampler2D _MainTex;
            float _AlphaCutoff;

            v2f vert(appdata_base v) {
                v2f o;
                TRANSFER_SHADOW_CASTER_NORMALOFFSET(o)
                o.uv = v.texcoord;
                return o;
            }

            float4 frag(v2f i) : SV_Target {
                //check if we have to drop the pixel
                float4 c = tex2D(_MainTex,i.uv);
                clip(c.a - _AlphaCutoff);
                //if not shadow cast
                SHADOW_CASTER_FRAGMENT(i)
            }
            ENDCG
        }

        //Real pass
        Pass{
            Tags{ "LightMode" = "ForwardBase" }
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            // make fog work
            #pragma multi_compile_fog
            // compile shader into multiple variants, with and without shadows
            // (we don't care about any lightmaps yet, so skip these variants)
            #pragma multi_compile_fwdbase nolightmap nodirlightmap nodynlightmap novertexlight
            // shadow helper functions and macros
            #include "AutoLight.cginc"
            #include "UnityCG.cginc"
            #include "UnityLightingCommon.cginc"

            struct v2f{
                float2 uv : TEXCOORD0;
                SHADOW_COORDS(1) // put shadows data into TEXCOORD1
                float4 pos : SV_POSITION;
                float4 diff : COLOR0; //diffuse lighting color
            };

            sampler2D _MainTex;
            float4 _MainTex_ST;
            float _Angle;
            float _Bright;
            float _Dark;
            float _AlphaCutoff;
            v2f vert (appdata_base v){
                v2f o;
                o.pos = UnityObjectToClipPos(v.vertex);
                //o.uv = TRANSFORM_TEX(v.uv, _MainTex);
                o.uv = v.texcoord;
                // get vertex normal in world space
                half3 worldNormal = UnityObjectToWorldNormal(v.normal);
                // dot product between normal and light direction for
                // standard diffuse (Lambert) lighting
                half nl = dot(worldNormal, _WorldSpaceLightPos0.xyz);
                //if nl is greater than a value we like set that value
                //if it is lower then decrease multiplicatively that value
                if (nl > _Angle) {
                    nl = _Bright;
                } else {
                    nl *= _Dark;
                }
                // factor in the light color
                o.diff = nl * _LightColor0;
                //ambient lightning
                o.diff.rgb += ShadeSH9(half4(worldNormal, 1));
                //Shadow
                TRANSFER_SHADOW(o)
                return o;
            }
            
            fixed4 frag (v2f i) : SV_Target	{
                // sample the texture
                fixed4 col = tex2D(_MainTex, i.uv);
                //check if we have to drop the pixel
                clip(col.a - _AlphaCutoff);
                // compute shadow attenuation (1.0 = fully lit, 0.0 = fully shadowed)
                fixed shadow = SHADOW_ATTENUATION(i);
                // multiply by lighting and receiving shadows
                col *= i.diff * shadow;
                return col;
            }
            ENDCG
        }

       
    }
}
