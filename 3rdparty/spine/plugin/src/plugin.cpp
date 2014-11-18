#include "plugin.h"
#include "skeleton.h"
#include "animation.h"
#include "event.h"
#include "slot.h"
#include "attachment.h"
#include "bone.h"

#include <spine/spine.h>
#include <spine/extension.h>

void _spAtlasPage_createTexture(spAtlasPage* self, const char* path)
{
	XTexture *texture = XGraphics::CreateTexture(path);
	if(texture)
	{
		self->rendererObject = texture;
		self->width = texture->getWidth();
		self->height = texture->getHeight();
	}
}

void _spAtlasPage_disposeTexture(spAtlasPage* self)
{
	if(self->rendererObject) {
		((XTexture*)self->rendererObject)->release();
	}
}

char* _spUtil_readFile(const char* path, int* length)
{
	string content;
	char *data = 0;
	if(XFileSystem::ReadFile(path, content))
	{
		*length = content.size();
		data = MALLOC(char, *length);
		memcpy(data, content.c_str(), *length);
	}
	return data;
}

int CreatePlugin(asIScriptEngine *scriptEngine)
{
	int r = 0;

	spBone_setYDown(true);
	
	r = scriptEngine->RegisterObjectType("spSkeleton", 0, asOBJ_REF); AS_ASSERT
	r = scriptEngine->RegisterObjectType("spAnimation", 0, asOBJ_REF); AS_ASSERT
	r = scriptEngine->RegisterObjectType("spAnimationStateData", 0, asOBJ_REF); AS_ASSERT
	r = scriptEngine->RegisterObjectType("spSlot", 0, asOBJ_REF); AS_ASSERT
	r = scriptEngine->RegisterObjectType("spRegionAttachment", 0, asOBJ_REF); AS_ASSERT
	r = scriptEngine->RegisterObjectType("spBone", 0, asOBJ_REF); AS_ASSERT
	r = spAnimationStateWrapper::TypeId = scriptEngine->RegisterObjectType("spAnimationState", 0, asOBJ_REF | asOBJ_GC); AS_ASSERT
	r = spEventWrapper::TypeId = scriptEngine->RegisterObjectType("spEvent", 0, asOBJ_REF); AS_ASSERT

	r = scriptEngine->RegisterFuncdef("void spEventCallback(spEvent@)"); AS_ASSERT
	
	r = scriptEngine->RegisterObjectBehaviour("spSkeleton", asBEHAVE_FACTORY, "spSkeleton @f(const string &in, const string &in, const float scale = 1.0f)", asFUNCTION(spSkeletonWrapper::Factory), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("spSkeleton", asBEHAVE_ADDREF, "void f()", asMETHOD(spSkeletonWrapper, addRef), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("spSkeleton", asBEHAVE_RELEASE, "void f()", asMETHOD(spSkeletonWrapper, release), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spSkeleton", "spAnimation @findAnimation(const string &in)", asMETHOD(spSkeletonWrapper, findAnimation), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spSkeleton", "spSlot @findSlot(const string &in)", asMETHOD(spSkeletonWrapper, findSlot), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spSkeleton", "spBone @findBone(const string &in)", asMETHOD(spSkeletonWrapper, findBone), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spSkeleton", "void set_position(const Vector2 &in)", asMETHOD(spSkeletonWrapper, setPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spSkeleton", "Vector2 get_position() const", asMETHOD(spSkeletonWrapper, getPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spSkeleton", "void set_flipX(const bool)", asMETHOD(spSkeletonWrapper, setFlipX), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spSkeleton", "bool get_flipX() const", asMETHOD(spSkeletonWrapper, getFlipX), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spSkeleton", "void set_flipY(const bool)", asMETHOD(spSkeletonWrapper, setFlipY), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spSkeleton", "bool get_flipY() const", asMETHOD(spSkeletonWrapper, getFlipY), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spSkeleton", "Texture @get_texture() const", asMETHOD(spSkeletonWrapper, getTexture), asCALL_THISCALL); AS_ASSERT
	//r = scriptEngine->RegisterObjectMethod("spSkeleton", "spAtlasPage getPage(const string &in) const", asMETHOD(spSkeletonWrapper, getPage), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spSkeleton", "void draw(Batch@)", asMETHOD(spSkeletonWrapper, draw), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectBehaviour("spAnimation", asBEHAVE_ADDREF, "void f()", asMETHOD(spAnimationWrapper, addRef), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("spAnimation", asBEHAVE_RELEASE, "void f()", asMETHOD(spAnimationWrapper, release), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimation", "void set_time(const float)", asMETHOD(spAnimationWrapper, setTime), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimation", "float get_time() const", asMETHOD(spAnimationWrapper, getTime), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimation", "void set_looping(const bool)", asMETHOD(spAnimationWrapper, setLooping), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimation", "bool get_looping() const", asMETHOD(spAnimationWrapper, getLooping), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimation", "void apply(const float)", asMETHOD(spAnimationWrapper, apply), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimation", "void mix(const float, const float)", asMETHOD(spAnimationWrapper, mix), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectBehaviour("spSlot", asBEHAVE_ADDREF, "void f()", asMETHOD(spSlotWrapper, addRef), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("spSlot", asBEHAVE_RELEASE, "void f()", asMETHOD(spSlotWrapper, release), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectBehaviour("spRegionAttachment", asBEHAVE_FACTORY, "spRegionAttachment @f(const string &in, spSlot@)", asFUNCTION(spRegionAttachmentWrapper::Factory), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("spRegionAttachment", asBEHAVE_ADDREF, "void f()", asMETHOD(spRegionAttachmentWrapper, addRef), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("spRegionAttachment", asBEHAVE_RELEASE, "void f()", asMETHOD(spRegionAttachmentWrapper, release), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spRegionAttachment", "void set_position(const Vector2 &in)", asMETHOD(spRegionAttachmentWrapper, setPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spRegionAttachment", "void set_rotation(const float)", asMETHOD(spRegionAttachmentWrapper, setRotation), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spRegionAttachment", "void set_scale(const Vector2 &in)", asMETHOD(spRegionAttachmentWrapper, setScale), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spRegionAttachment", "void set_size(const Vector2 &in)", asMETHOD(spRegionAttachmentWrapper, setSize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spRegionAttachment", "void set_textureRegion(const TextureRegion &in)", asMETHOD(spRegionAttachmentWrapper, setTextureRegion), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectBehaviour("spBone", asBEHAVE_ADDREF, "void f()", asMETHOD(spBoneWrapper, addRef), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("spBone", asBEHAVE_RELEASE, "void f()", asMETHOD(spBoneWrapper, release), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spBone", "void set_position(const Vector2 &in)", asMETHOD(spBoneWrapper, setPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spBone", "void set_rotation(const float)", asMETHOD(spBoneWrapper, setRotation), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spBone", "void set_scale(const Vector2 &in)", asMETHOD(spBoneWrapper, setScale), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spBone", "Vector2 get_worldPosition() const", asMETHOD(spBoneWrapper, getWorldPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spBone", "float get_worldRotation() const", asMETHOD(spBoneWrapper, getWorldRotation), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spBone", "Vector2 get_worldScale() const", asMETHOD(spBoneWrapper, getWorldScale), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterEnum("spEventType"); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("spEventType", "SP_ANIMATION_START", SP_ANIMATION_START); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("spEventType", "SP_ANIMATION_END", SP_ANIMATION_END); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("spEventType", "SP_ANIMATION_COMPLETE", SP_ANIMATION_COMPLETE); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("spEventType", "SP_ANIMATION_EVENT", SP_ANIMATION_EVENT); AS_ASSERT
	
	r = scriptEngine->RegisterObjectBehaviour("spEvent", asBEHAVE_ADDREF, "void f()", asMETHOD(spEventWrapper, addRef), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("spEvent", asBEHAVE_RELEASE, "void f()", asMETHOD(spEventWrapper, release), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spEvent", "string get_string() const", asMETHOD(spEventWrapper, getString), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spEvent", "int get_int() const", asMETHOD(spEventWrapper, getInt), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spEvent", "float get_float() const", asMETHOD(spEventWrapper, getFloat), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spEvent", "spEventType get_type() const", asMETHOD(spEventWrapper, getType), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spEvent", "int get_loopCount() const", asMETHOD(spEventWrapper, getLoopCount), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectBehaviour("spAnimationStateData", asBEHAVE_FACTORY, "spAnimationStateData @f(spSkeleton@)", asFUNCTION(spAnimationStateDataWrapper::Factory), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("spAnimationStateData", asBEHAVE_ADDREF, "void f()", asMETHOD(spAnimationStateDataWrapper, addRef), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("spAnimationStateData", asBEHAVE_RELEASE, "void f()", asMETHOD(spAnimationStateDataWrapper, release), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimationStateData", "void getMix(spAnimation@, spAnimation@) const", asMETHOD(spAnimationStateDataWrapper, getMix), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimationStateData", "void setMix(spAnimation@, spAnimation@, const float)", asMETHODPR(spAnimationStateDataWrapper, setMix, (spAnimationWrapper*, spAnimationWrapper*, const float), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimationStateData", "void setMix(const string &in, const string &in, const float)", asMETHODPR(spAnimationStateDataWrapper, setMix, (const string&, const string&, const float), void), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectBehaviour("spAnimationState", asBEHAVE_FACTORY, "spAnimationState @f(spAnimationStateData@)", asFUNCTION(spAnimationStateWrapper::Factory), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("spAnimationState", asBEHAVE_ADDREF, "void f()", asMETHOD(spAnimationStateWrapper, addRef), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("spAnimationState", asBEHAVE_RELEASE, "void f()", asMETHOD(spAnimationStateWrapper, release), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("spAnimationState", asBEHAVE_SETGCFLAG, "void f()", asMETHOD(spAnimationStateWrapper, setGCFlag), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("spAnimationState", asBEHAVE_GETGCFLAG, "bool f()", asMETHOD(spAnimationStateWrapper, getGCFlag), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("spAnimationState", asBEHAVE_GETREFCOUNT, "int f()", asMETHOD(spAnimationStateWrapper, getRefCount), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("spAnimationState", asBEHAVE_ENUMREFS, "void f(int&in)", asMETHOD(spAnimationStateWrapper, enumReferences), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("spAnimationState", asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(spAnimationStateWrapper, releaseReferences), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimationState", "void set_looping(const bool)", asMETHOD(spAnimationStateWrapper, setLooping), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimationState", "bool get_looping() const", asMETHOD(spAnimationStateWrapper, getLooping), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimationState", "void set_timeScale(const float)", asMETHOD(spAnimationStateWrapper, setTimeScale), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimationState", "float get_timeScale() const", asMETHOD(spAnimationStateWrapper, getTimeScale), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimationState", "void set_eventCallback(spEventCallback@)", asMETHOD(spAnimationStateWrapper, setEventCallback), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimationState", "spEventCallback@ get_eventCallback() const", asMETHOD(spAnimationStateWrapper, getEventCallback), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimationState", "void setAnimation(spAnimation@)", asMETHODPR(spAnimationStateWrapper, setAnimation, (spAnimationWrapper*), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimationState", "void setAnimation(const string &in)", asMETHODPR(spAnimationStateWrapper, setAnimation, (const string&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimationState", "void addAnimation(spAnimation@, const float)", asMETHODPR(spAnimationStateWrapper, addAnimation, (spAnimationWrapper*, const float), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimationState", "void addAnimation(const string &in, const float)", asMETHODPR(spAnimationStateWrapper, addAnimation, (const string&, const float), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("spAnimationState", "void update(const float)", asMETHOD(spAnimationStateWrapper, update), asCALL_THISCALL); AS_ASSERT

	return r;
}

void ReleasePlugin()
{
}