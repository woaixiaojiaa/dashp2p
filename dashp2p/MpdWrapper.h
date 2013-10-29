/****************************************************************************
 * MpdWrapper.h                                                             *
 ****************************************************************************
 * Copyright (C) 2012 Technische Universitaet Berlin                        *
 *                                                                          *
 * Created on: Sep 11, 2012                                                 *
 * Authors: Konstantin Miller <konstantin.miller@tu-berlin.de>              *
 *                                                                          *
 * This program is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * This program is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.     *
 ****************************************************************************/

#ifndef MPDWRAPPER_H_
#define MPDWRAPPER_H_


//#include "Dashp2pTypes.h"
#include "mpd/model.h"
#include <cassert>
#include <map>
#include <vector>
#include <string>

using std::pair;
using std::map;
using std::vector;
using std::string;

namespace dashp2p {

class MpdId;
class PeriodId;
class AdaptationSetId;
class RepresentationId;
class SegmentId;

class MpdWrapper
{
/* Public methods */
public:
	/**
	 * @param p     Pointer to memory holding the MPD file. Will be deleted within this function.
	 * @param size  Size of the MPD file.
	 */
    MpdWrapper(char* p, int size);
    virtual ~MpdWrapper() {delete mpd;}

    /**********************************************************************
     * Properties of the MPD **********************************************
     **********************************************************************/

    /**
     * Returns the ID of the MPD or an empty string if the ID is not set.
     */
    string getMpdId() const {if(mpd->id.isSet()) return mpd->id.get(); else return string();}
    int64_t getVideoDuration() const;

    /**********************************************************************
     * Properties of a Period *********************************************
     **********************************************************************/

    /**********************************************************************
     * Properties of an Adaptation Set ************************************
     **********************************************************************/

    int getNumRepresentations(const AdaptationSetId& adaptationSetId) const;
    int getNumRepresentations(int periodIndex, int adaptationSetIndex) const;
    int getNumRepresentations(int periodIndex, int adaptationSetIndex, int width, int height) const;

    /**
     * Retrievs all representation of a given adaptation set.
     */
    vector<RepresentationId> getRepresentations(const AdaptationSetId& adaptationSetId) const;

    /**
     * Retrievs all representation of a given adaptation set that match the specified spatial resolution width x height.
     * @param width   Horizontal resolution. Must be strictly greater than zero.
     * @param height  Vertical resolution. Must be strictly greater than zero.
     */
    vector<RepresentationId> getRepresentations(const AdaptationSetId& adaptationSetId, int width, int height) const;
    RepresentationId getRepresentationIdByBitrate(const AdaptationSetId& adaptationSetId, int bitRate) const;
    vector<int> getBitrates(int periodIndex, int adaptationSetIndex, int width, int height) const;
    vector<pair<int, int> > getSpatialResolutions(int periodIndex, int adaptationSetIndex) const;
    pair<int,int> getLowestSpatialResolution(int periodIndex, int adaptationSetIndex) const;
    pair<int,int> getHighestSpatialResolution(int periodIndex, int adaptationSetIndex) const;
    string printSpatialResolutions(int periodIndex, int adaptationSetIndex) const;

    /**********************************************************************
     * Properties of a Representation *************************************
     **********************************************************************/

    int getNumSegments(int periodIndex, int adaptationSetIndex, int representationIndex) const;
    int getNumSegments(const dashp2p::mpd::Representation& rep) const;
    int getNumSegments(const RepresentationId& representationId) const;
    vector<SegmentId> getSegments(const RepresentationId& representationId) const;
    int64_t getNominalSegmentDuration(int periodIndex, int adaptationSetIndex, int representationIndex) const;
    int64_t getNominalSegmentDuration(const dashp2p::mpd::Representation& rep) const;
    string getInitSegmentURL(const dashp2p::mpd::Representation& rep) const;
    dashp2p::URL getInitSegmentUrl(const RepresentationId& representationId) const;
    ContentIdSegment getNextSegment(const ContentIdSegment& segId) const;
    pair<int, int> getSpatialResolution(const RepresentationId& representationId) const;
    int getBitrate(const RepresentationId& representationId) const;
    int getWidth(const RepresentationId& representationId) const;
    int getHeight(const RepresentationId& representationId) const;

    /**********************************************************************
     * Properties of a Segment ********************************************
     **********************************************************************/

    int64_t getSegmentDuration(const ContentIdSegment& setId) const;
    int64_t getSegmentDuration(const dashp2p::mpd::Representation& rep, int segmentIndex) const;
    int64_t getSegmentDuration(int periodIndex, int adaptationSetIndex, int representationIndex, int segmentIndex) const;
    int64_t getPosition(const ContentIdSegment& segId, int64_t byte, int64_t segmentSize) const;
    int64_t getStartTime(const ContentIdSegment& segId) const;
    int64_t getEndTime(const ContentIdSegment& segId) const;

    /**
     * Returns the URL of a segment.
     */
    string getSegmentURL(const ContentIdSegment& segId) const;
    dashp2p::URL getSegmentUrl(const SegmentId& segmentId) const;





    //std::vector<int64_t> getSwitchingPoints(int64_t streamPosition, int num) const;
    void outputVideoStatistics(const string& fileName) const;

/* Privave methods */
private:
    const dashp2p::mpd::Period& getPeriod(int periodIndex) const;
    const dashp2p::mpd::Period& getPeriod(const PeriodId& periodId) const;
    const dashp2p::mpd::AdaptationSet& getAdaptationSet(int periodIndex, int adaptationSetIndex) const;
    const dashp2p::mpd::AdaptationSet& getAdaptationSet(const AdaptationSetId& adaptationSetId) const;
    const dashp2p::mpd::Representation& getRepresentation(const RepresentationId& representationId) const;
    const dashp2p::mpd::Representation& getRepresentation(int periodIndex, int adaptationSetIndex, int representationIndex) const;
    const dashp2p::mpd::Representation& getRepresentationByBitrate(int periodIndex, int adaptationSetIndex, int bitRate) const;

/* Private members */
private:
    dashp2p::mpd::MediaPresentationDescription* mpd;
};


/********************************************************************
 * ID classes *******************************************************
 ********************************************************************/

class MpdId {
public:
	bool operator<(const MpdId& other) const {return (mpdId.compare(other.mpdId) < 0);}
	string mpdId;
};

class PeriodId: public MpdId {
public:
	PeriodId(int periodIndex): periodIndex(periodIndex) {}
	bool operator<(const PeriodId& other) const;
	int periodIndex;
};

class AdaptationSetId: public PeriodId {
public:
	AdaptationSetId(int periodIndex, int adaptationSetIndex): PeriodId(periodIndex), adaptationSetIndex(adaptationSetIndex) {}
	bool operator<(const AdaptationSetId& other) const;
	int adaptationSetIndex;
};

class RepresentationId: public AdaptationSetId {
public:
	RepresentationId(const AdaptationSetId& adaptationSetId, int representationIndex): AdaptationSetId(adaptationSetId), representationIndex(representationIndex) {}
	bool operator<(const RepresentationId& other) const;
	int representationIndex;
};

class SegmentId: public RepresentationId {
public:
	SegmentId(const RepresentationId& representationId, int segmentIndex): RepresentationId(representationId), segmentIndex(segmentIndex) {}
	bool operator<(const SegmentId& other) const;
	bool isInitSegment() const {return (segmentIndex == 0);}
	int segmentIndex;
};

}

#endif /* MPDWRAPPER_H_ */
