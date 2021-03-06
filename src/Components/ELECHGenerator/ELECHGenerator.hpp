/*!
 * \file
 * \brief
 * \author Micha Laszkowski
 */

#ifndef ELECHGenerator_HPP_
#define ELECHGenerator_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <Types/PointXYZSIFT.hpp>
#include <Types/SIFTObjectModel.hpp>
#include <Types/SIFTObjectModelFactory.hpp>
#include <Types/MergeUtils.hpp>


#include <pcl/registration/correspondence_estimation.h>
#include "pcl/registration/correspondence_rejection_sample_consensus.h"
#include <pcl/registration/elch.h>
#include <pcl/registration/lum.h>

#include <opencv2/core/core.hpp>

namespace Processors {
namespace ELECHGenerator {

/*!
 * \class ELECHGenerator
 * \brief ELECHGenerator processor class.
 *
 * ELECHGenerator processor.
 */
class ELECHGenerator: public Base::Component,SIFTObjectModelFactory {
public:
	/*!
	 * Constructor.
	 */
    ELECHGenerator(const std::string & name = "ELECHGenerator");

	/*!
	 * Destructor
	 */
    virtual ~ELECHGenerator();

	/*!
	 * Prepare components interface (register streams and handlers).
	 * At this point, all properties are already initialized and loaded to
	 * values set in config file.
	 */
	void prepareInterface();

protected:

	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();

	/// Input data stream containing point cloud from a given view.
	Base::DataStreamIn<pcl::PointCloud<pcl::PointXYZRGB>::Ptr> in_cloud_xyzrgb;

	/// Input data stream containing feature cloud from a given view.
	Base::DataStreamIn<pcl::PointCloud<PointXYZSIFT>::Ptr> in_cloud_xyzsift;

	//Base::DataStreamIn<bool> in_trigger;

//	Base::DataStreamOut<ELECHGenerator> out_Trigger;

	/// Output data stream containing SIFTObjectModel - depricated.
	Base::DataStreamOut<AbstractObject*> out_instance;

	/// Output data stream containing object model point cloud.
	Base::DataStreamOut<pcl::PointCloud<pcl::PointXYZRGB>::Ptr> out_cloud_xyzrgb;

	/// Input data stream containing point cloud with normals from a given view.
	Base::DataStreamIn<pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr> in_cloud_xyzrgb_normals;

	/// Output data stream containing object model feature cloud (SIFTs).
	Base::DataStreamOut<pcl::PointCloud<PointXYZSIFT>::Ptr> out_cloud_xyzsift;
	
	Base::DataStreamOut<pcl::PointCloud<PointXYZSIFT>::Ptr> out_cloud_lastsift;
	Base::DataStreamOut<pcl::PointCloud<PointXYZSIFT>::Ptr> out_cloud_lastsift2;

	// Mean number of features per view.
	Base::DataStreamOut<int> out_mean_viewpoint_features_number;


	// Handlers
    Base::EventHandler2 h_addViewToModelNormal;
    Base::EventHandler2 h_addViewToModel;
    Base::EventHandler2 h_Trigger;

	// Handlers
    void addViewToModelNormal();
    void addViewToModel();

    bool loopDetection(int end, std::vector<pcl::PointCloud<pcl::PointXYZRGB>::Ptr> &clouds, double dist, int &first, int &last);

    MergeUtils::Properties properties;

	/// Number of views.
	int counter;

	/// Total number of features (in all views).
	int total_viewpoint_features_number;

	std::vector<pcl::PointCloud<pcl::PointXYZRGB>::Ptr> rgb_views;
	std::vector<pcl::PointCloud<PointXYZSIFT>::Ptr> sift_views;
	pcl::registration::ELCH<pcl::PointXYZRGB> elch_rgb;
	pcl::registration::ELCH<PointXYZSIFT> elch_sift;
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_merged;
	pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr cloud_normal_merged;
	pcl::PointCloud<PointXYZSIFT>::Ptr cloud_sift_merged;
	Eigen::Matrix4f global_trans;

public:
    Base::Property<bool> prop_ICP_alignment;
    Base::Property<bool> prop_ICP_alignment_color;
    
    Base::Property<double> ICP_transformation_epsilon;
    Base::Property<float> Elch_max_correspondence_distance;
    Base::Property<float> Elch_ICP_max_iterations;

    ///RanSAC Properties
    Base::Property<float> RanSAC_inliers_threshold;
    Base::Property<float> RanSAC_max_iterations;

    /// Alignment mode: use ICP alignment or not.
	/// ICP properties
    Base::Property<float> Elch_loop_dist;
    Base::Property<float> ICP_max_correspondence_distance;
    Base::Property<float> ICP_max_iterations;
    Base::Property<float> Elch_rejection_threshold;

};
/*
 * Register processor component.
 */
REGISTER_COMPONENT("ELECHGenerator", Processors::ELECHGenerator::ELECHGenerator)

} //: namespace ELECHGenerator
} //: namespace Processors

#endif /* ELECHGenerator_HPP_ */
