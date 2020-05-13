using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Will apply te OutlineEffect, only in the visible objects if the camera is
/// setup as It was mean to
/// </summary>
[ExecuteInEditMode]
public class OutlineEffect : MonoBehaviour
{
    [SerializeField]
    protected List<GameObject> visibleObjects = new List<GameObject>();
    [SerializeField]
    protected float drawDistance = 0.01f;
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
    //remember to this to work the camera needs to cull the invisible layer and render
    protected int invisibleLayer = 11;
    protected int toRenderLayer = 12;

    protected void Awake()
    {
        outlineMat = new Material(Shader.Find("Custom/OutlineEffectShader"));

    }

    protected void Start()
    {
        cam = GetComponent<Camera>();
        cam.depthTextureMode = DepthTextureMode.DepthNormals;
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
        foreach (var go in visibleObjects)
        {
            go.layer = rendLayer;
        }
    }

    private void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        pixelToUV.x = cam.scaledPixelWidth;
        pixelToUV.y = cam.scaledPixelHeight;
        outlineMat.SetVector("_PixelToUVFactor", pixelToUV);
        outlineMat.SetFloat("_DistanceToDraw", drawDistance);
        Graphics.Blit(source, destination, outlineMat);
    }
}
