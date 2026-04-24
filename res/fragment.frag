#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform vec4 ourColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform float xPos;
uniform float yPos;


void main()
{
  vec4 wallColor = texture(texture0, TexCoord);

  vec2 pixelCoords = gl_FragCoord.xy;
  vec2 mousePos = vec2(xPos, yPos);
  float lightSize = 200.0;

  vec2 lightUV = (pixelCoords.xy - mousePos) / lightSize + vec2(0.5, 0.5);
  vec4 lightCookie = texture(texture1, lightUV);

  FragColor = wallColor * lightCookie * 2.0;
}