/*
**
** Copyright 2013, Samsung Electronics Co. LTD
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef EXYNOS_CAMERA_PIPE_FLITE_H
#define EXYNOS_CAMERA_PIPE_FLITE_H

#include "ExynosCameraPipe.h"

namespace android {

typedef ExynosCameraList<ExynosCameraFrame *> frame_queue_t;

class ExynosCameraPipeFlite : protected virtual ExynosCameraPipe {
public:
    ExynosCameraPipeFlite(
        int cameraId,
        ExynosCameraParameters *obj_param,
        bool isReprocessing,
        int32_t *nodeNums);

    virtual ~ExynosCameraPipeFlite();

    virtual status_t        create(int32_t *sensorIds);
    virtual status_t        destroy(void);

    virtual status_t        setupPipe(camera_pipe_info_t *pipeInfos);

    virtual status_t        sensorStream(bool on);

private:
    virtual bool            m_mainThreadFunc(void);
    virtual status_t        m_getBuffer(void);
#ifdef SHOT_RECOVERY
    int                     retryGetBufferCount;
#endif
};

}; /* namespace android */

#endif
