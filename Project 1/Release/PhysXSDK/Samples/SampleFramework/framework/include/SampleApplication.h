// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2013 NVIDIA Corporation. All rights reserved.


#ifndef SAMPLE_APPLICATION_H
#define SAMPLE_APPLICATION_H
/*
 * Copyright 2008-2012 NVIDIA Corporation.  All rights reserved.
 *
 * NOTICE TO USER:
 *
 * This source code is subject to NVIDIA ownership rights under U.S. and
 * international Copyright laws.  Users and possessors of this source code
 * are hereby granted a nonexclusive, royalty-free license to use this code
 * in individual and commercial software.
 *
 * NVIDIA MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS SOURCE
 * CODE FOR ANY PURPOSE.  IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR
 * IMPLIED WARRANTY OF ANY KIND.  NVIDIA DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOURCE CODE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS,  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION,  ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OR PERFORMANCE OF THIS SOURCE CODE.
 *
 * U.S. Government End Users.   This source code is a "commercial item" as
 * that term is defined at  48 C.F.R. 2.101 (OCT 1995), consisting  of
 * "commercial computer  software"  and "commercial computer software
 * documentation" as such terms are  used in 48 C.F.R. 12.212 (SEPT 1995)
 * and is provided to the U.S. Government only as a commercial end item.
 * Consistent with 48 C.F.R.12.212 and 48 C.F.R. 227.7202-1 through
 * 227.7202-4 (JUNE 1995), all U.S. Government End Users acquire the
 * source code with only those rights set forth herein.
 *
 * Any use of this source code in individual and commercial software must
 * include, in the user documentation and internal comments to the code,
 * the above Disclaimer and U.S. Government End Users Notice.
 */
#include <Renderer.h>
#include <RendererWindow.h>

#include <FrameworkFoundation.h>

#include <SampleAssetManager.h>
#include <PsTime.h>
#include "foundation/PxMat44.h"
#include <SampleUserInput.h>

namespace physx { 
	class PxProfileZone;
	class PxProfileZoneManager;
	class PxProfileEventSender;
}

#define SMOOTH_CAM

namespace SampleFramework
{

	class SampleCommandLine;

	template <typename T>
	class Transform
	{
	public:
		Transform() : m_dirty(true) {}

		void         setForwardTransform( const T& t )
		{ 
			m_fwd = t;
			m_dirty = true;
		}

		void         setInverseTransform( const T& t )
		{ 
			m_inv = t;
			m_fwd = t.inverseRT();
			m_dirty = false;
		}

		const T&	getForwardTransform() const
		{ 
			return m_fwd;
		}

		const T&	getInverseTransform() const
		{
			if(m_dirty)
			{
				m_inv = m_fwd.inverseRT();
				m_dirty = false;
			}
			return m_inv;
		}

	private:
		T    m_fwd;
		mutable T    m_inv;
		mutable bool m_dirty;
	};

	class SampleApplication : public SampleRenderer::RendererWindow, public InputEventListener
	{
	public:
		SampleApplication(const SampleCommandLine& cmdline, const char* assetPathPrefix="media", PxI32 camMoveButton = -1);
		virtual								~SampleApplication(void);
		SampleRenderer::Renderer*	getRenderer(void)								{ return m_renderer; }
		SampleAssetManager*			getAssetManager(void)							{ return m_assetManager; }
		inline const char*			getAssetPathPrefix(void)				const	{ return m_assetPathPrefix; }

		const PxMat44&				getEyeTransform(void)					const	{ return m_worldToView.getInverseTransform(); }
		void						setEyeTransform(const PxMat44& eyeTransform);
		void						setEyeTransform(const PxVec3& pos, const PxVec3& rot);
		void						setViewTransform(const PxMat44& viewTransform);
		const PxMat44&				getViewTransform(void)					const;

		const SampleCommandLine&	getCommandLine(void)					const	{ return m_cmdline; }

		virtual	void						onInit(void) = 0;
		virtual	void						onShutdown(void) = 0;

		virtual	float						tweakElapsedTime(float dtime)	{ return dtime;	}
		virtual	void						onTickPreRender(float dtime) = 0;
		virtual	void						onRender(void) = 0;
		virtual	void						onTickPostRender(float dtime) = 0;

		virtual	void						onOpen(void);
		virtual	bool						onClose(void);

		virtual	void						onDraw(void);

		virtual void						onAnalogInputEvent(const InputEvent& , float val);
		virtual bool						onDigitalInputEvent(const InputEvent& , bool val);
		virtual void						onPointerInputEvent(const InputEvent&, physx::PxU32 x, physx::PxU32 y, physx::PxReal dx, physx::PxReal dy, bool val);

		virtual void						rotateCamera(PxF32 dx, PxF32 dy);

		virtual	void						doInput(void);
		virtual void						fatalError(const char * msg);



		//Must be called when foundation is setup.
		virtual physx::PxProfileZone*			getOrCreateProfileZone(PxFoundation& inFoundation);
		virtual physx::PxProfileZone*			getOrCreateProfileZone(physx::PxProfileZoneManager& zoneManager);
		virtual physx::PxProfileZone*			getProfileZone() { return mProfileZone; }
		//Must be called when foundation is setup.
		virtual void							releaseProfileZone();
		virtual physx::PxProfileEventSender*	getEventBuffer();

	protected:
		const SampleCommandLine&			m_cmdline;

		physx::PxProfileZone*				mProfileZone;

		PxF32						m_sceneSize;

		SampleRenderer::Renderer*	m_renderer;

		char						m_assetPathPrefix[256];
		SampleAssetManager*			m_assetManager;

		PxVec3						m_eyeRot;
		Transform<PxMat44>			m_worldToView;

		PxI32						m_camMoveButton;

#if defined(SMOOTH_CAM)
		PxVec3						m_targetEyeRot;
		PxVec3						m_targetEyePos;
#endif
		physx::PxU64				m_timeCounter;
		physx::shdfnd::Time			m_time;
		bool						m_disableRendering;

		void*						android_window_ptr;
	private:
		// get rid of C4512
		void						operator=(const SampleApplication&);
	};

} //namespace SampleFramework

#endif
