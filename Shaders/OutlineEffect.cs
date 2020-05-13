using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


namespace form.legend
{
    public class OutlineEffect : MonoBehaviour
    {
        protected List<GameObject> affectedObjects = new List<GameObject>();
        protected Color outlineColor;
        public Color OutlineColor
        {
            get => outlineColor;
            set
            {
                outlineMat.SetColor("_Color", value);
                outlineColor = value;
            }
        }
        protected Camera cam;
        protected Material outlineMat;
        protected Vector4 pixelToUV = Vector4.zero;
        protected int invisibleLayer = 11;
        protected int toRenderLayer = 12;

        protected void Awake()
        {
            outlineMat = new Material(Shader.Find("Custom/OutlineEffectShader"));

        }

        protected void Start()
        {
            cam = GetComponent<Camera>();
            outlineColor = Color.black;
        }


        private void OnPreCull()
        {
            ChangeLayerObjects(toRenderLayer);
        }

        private void OnPostRender()
        {
            ChangeLayerObjects(invisibleLayer);
        }

        private void ChangeLayerObjects(int rendLayer)
        {
            foreach(var go in affectedObjects)
            {
                go.layer = rendLayer;
            }
        }

        private void OnRenderImage(RenderTexture source, RenderTexture destination)
        {
            pixelToUV.x = cam.scaledPixelWidth;
            pixelToUV.y = cam.scaledPixelHeight;
            outlineMat.SetVector("_PixelToUVFactor", pixelToUV);
            Graphics.Blit(source, destination, outlineMat);
        }
    }
}