extern GLint h_aPosition, h_aNormal, h_uViewMatrix, h_uProjMatrix;
extern GLuint CubeBuffObj, CIndxBuffObj, GrndBuffObj, GIndxBuffObj, GNBuffObj, GNIndxBuffObj;
extern GLuint ShadowCubeBuffObj, SCIndxBuffObj, ShadowNormalBuffObj, RampBuffObj, RIndxBuffObj, RampNormalBuffObj;
extern GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine;
extern float g_width, g_height, beta, alpha, lookAtx, lookAty, lookAtz;
extern glm::vec3 eye, lookAtPoint, up;
extern int ShadeProg;
extern GLint h_uLightVec;
extern GLint h_uLightColor;
extern GLint h_uCamPos, h_uShadeMode;
extern GLint h_uModelMatrix;
extern RenderingHelper ModelTrans;

int InstallShader(const GLchar *vShaderName, const GLchar *fShaderName);
void glInitialize(void);
