#pragma once
#include "util.hpp"

#include <dr_camera/intensity_camera.hpp>
#include <dr_camera/depth_camera.hpp>
#include <dr_camera/point_cloud_camera.hpp>
#include <dr_camera_parameters/intrinsic_parameters.hpp>

#include <ensenso/nxLib.h>
#include <boost/optional.hpp>

namespace dr {

class Ensenso : public IntensityCamera, public PointCloudCamera {
public:
	Ensenso(bool connect_overlay = true);

	~Ensenso();

	/// Returns the pose of the camera with respect to the calibration plate.
	bool calibrate(int const num_patterns, Eigen::Isometry3d & pose) const;

	/// Returns the size of the intensity images.
	cv::Size getIntensitySize() override;

	/// Returns the size of the depth images.
	cv::Size getPointCloudSize() override;

	/// Loads the intensity image to intensity.
	void loadIntensity(cv::Mat & intensity) override;

	/// Loads the camera parameters from a JSON file.
	void loadParameters(std::string const parameters_file);

	/**
	 * Loads the pointcloud from depth in the region of interest.
	 * \param cloud the resulting pointcloud.
	 * \param roi The region of interest.
	 */
	void loadPointCloud(PointCloudCamera::PointCloud & cloud, cv::Rect roi = cv::Rect()) override;

	NxLibItem getNativeCamera() {
		return ensenso_camera;
	}

	boost::optional<NxLibItem> getNativeOverlayCamera() {
		if (found_overlay) {
			return overlay_camera;
		} else {
			return boost::none;
		}
	}

	std::string getOverlaySerialNumber() {
		return getNx<std::string>(overlay_camera[itmSerialNumber]);
	}

	std::string getSerialNumber() {
		return getNx<std::string>(ensenso_camera[itmSerialNumber]);
	}

protected:
	/// The root EnsensoSDK node.
	NxLibItem root;

	/// The Ensenso camera node.
	NxLibItem ensenso_camera;

	/// True if a connected overlay camera is found.
	bool found_overlay;

	/// The overlay camera node.
	NxLibItem overlay_camera;

	/// Set the region of interest for the disparity map (and thereby depth / point cloud).
	void setRegionOfInterest(cv::Rect const & roi);

};

}
