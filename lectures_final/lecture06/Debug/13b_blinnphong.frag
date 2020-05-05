/*
13b_blinnphong.frag: Fragment shader for the Blinn-Phong illumination model

N.B.)  "13_phong.vert" must be used as vertex shader

author: Davide Gadia

Real-Time Graphics Programming - a.a. 2018/2019
Master degree in Computer Science
Universita' degli Studi di Milano

*/

#version 330 core

// output shader variable
out vec4 colorFrag;

// light incidence direction (calculated in vertex shader, interpolated by rasterization)
in vec3 lightDir;
// the transformed normal has been calculated per-vertex in the vertex shader
in vec3 vNormal;
// vector from fragment to camera (in view coordinate)
in vec3 vViewPosition;


// ambient, diffusive and specular components (passed from the application)
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
// weight of the components
// in this case, we can pass separate values from the main application even if Ka+Kd+Ks>1. In more "realistic" situations, I have to set this sum = 1, or at least Kd+Ks = 1, by passing Kd as uniform, and then setting Ks = 1.0-Kd
uniform float Ka;
uniform float Kd;
uniform float Ks;
// attenuation parameters
uniform float constant;
uniform float linear;
uniform float quadratic;
// shininess coefficients (passed from the application)
uniform float shininess;


void main(){

    // ambient component can be calculated at the beginning
    vec3 color = Ka*ambientColor;

    // normalization of the per-fragment normal
    vec3 N = normalize(vNormal);
    // we take the distance from the light source (before normalization, for the attenuation parameter)
    float distanceL = length(lightDir.xyz);
    // normalization of the per-fragment light incidence direction
    vec3 L = normalize(lightDir.xyz);

    // we calculate the attenuation factor (based on the distance from light source)
    float attenuation = 1.0/(constant + linear*distanceL + quadratic*(distanceL*distanceL));

    // Lambert coefficient
    float lambertian = max(dot(L,N), 0.0);

    // if the lambert coefficient is positive, then I can calculate the specular component
    if(lambertian > 0.0)
    {
        // the view vector has been calculated in the vertex shader, already negated to have direction from the mesh to the camera
        vec3 V = normalize( vViewPosition );

        // in the Blinn-Phong model we do not use the reflection vector, but the half vector
        vec3 H = normalize(L + V);

        // we use H to calculate the specular component
        float specAngle = max(dot(H, N), 0.0);
        // shininess application to the specular component
        float specular = pow(specAngle, shininess);

        // We add diffusive and specular components to the final color
        // N.B. ): in this implementtion, the sum of the components can be different than 1
        color += vec3( Kd * lambertian * diffuseColor +
                        Ks * specular * specularColor);
        color*=attenuation;

      }

      colorFrag  = vec4(color,1.0);

}
