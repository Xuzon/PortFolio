Shader "Custom/OutlineEffectShader"
{
    Properties
    {
        _MainTex("Texture", 2D) = "white" {}
        _Color("Color", Color) = (0,0,0,1)
        _PixelToUVFactor("Pixel factor", Vector) = (0.000520833, 0.0008333, 0, 0)
        _DistanceToDraw("Disance diff to draw", Float) = 0.01
    }
        SubShader
        {
            // No culling or depth
            Cull Off ZWrite Off ZTest Always

            Pass
            {
                CGPROGRAM
                #pragma vertex vert
                #pragma fragment frag

                #include "UnityCG.cginc"

                struct appdata
                {
                    float4 vertex : POSITION;
                    float2 uv : TEXCOORD0;
                };

                struct v2f
                {
                    float2 uv : TEXCOORD0;
                    float4 vertex : SV_POSITION;
                };

                v2f vert(appdata v)
                {
                    v2f o;
                    o.vertex = UnityObjectToClipPos(v.vertex);
                    o.uv = v.uv;
                    return o;
                }

                sampler2D _MainTex;
                float4 _Color;
                float4 _PixelToUVFactor;
                float _DistanceToDraw;
                sampler2D _CameraDepthNormalsTexture;

                fixed4 frag(v2f i) : SV_Target
                {
                    float2 fac = float2(1 / _PixelToUVFactor.x, 1 / _PixelToUVFactor.y);
                    float2 leftUV = i.uv;
                    leftUV.x -= fac.x;
                    float2 rightUV = i.uv;
                    rightUV.x += fac.x;

                    float2 topUV = i.uv;
                    topUV.y += fac.y;
                    float2 bottomUV = i.uv;
                    bottomUV.y -= fac.y;


                    fixed4 col = tex2D(_MainTex, i.uv);
                    float depth = DecodeFloatRG(tex2D(_CameraDepthNormalsTexture, i.uv).ba);
                    float left = DecodeFloatRG(tex2D(_CameraDepthNormalsTexture, leftUV).ba);
                    float right = DecodeFloatRG(tex2D(_CameraDepthNormalsTexture, rightUV).ba);
                    float top = DecodeFloatRG(tex2D(_CameraDepthNormalsTexture, topUV).ba);
                    float bottom = DecodeFloatRG(tex2D(_CameraDepthNormalsTexture, bottomUV).ba);


                    if (_DistanceToDraw < abs(depth - left) || _DistanceToDraw < abs(depth - right)
                    || _DistanceToDraw < abs(depth - top) || _DistanceToDraw < abs(depth - bottom))
                    {
                        return _Color;
                    }

                    return col;
                }
                ENDCG
            }
        }
}