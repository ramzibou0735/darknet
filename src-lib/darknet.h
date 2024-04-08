#pragma once

#if defined(_MSC_VER) && _MSC_VER < 1900
#define inline __inline
#endif

#if defined(DEBUG) && !defined(_CRTDBG_MAP_ALLOC)
#define _CRTDBG_MAP_ALLOC
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <pthread.h>

/// @todo what is this?
#define SECRET_NUM -1234

typedef enum { UNUSED_DEF_VAL } UNUSED_ENUM_TYPE;

#ifdef GPU

#include <cuda_runtime.h>
#include <curand.h>
#include <cublas_v2.h>

#ifdef CUDNN
#include <cudnn.h>
#endif  // CUDNN
#endif  // GPU

#ifdef __cplusplus
extern "C" {
#endif

struct network;
typedef struct network network;

struct network_state;
typedef struct network_state network_state;

struct layer;
typedef struct layer layer;

struct image;
typedef struct image image;

struct detection;
typedef struct detection detection;

struct load_args;
typedef struct load_args load_args;

struct data;
typedef struct data data;

struct metadata;
typedef struct metadata metadata;

struct tree;
typedef struct tree tree;

// option_list.h
typedef struct metadata {
	int classes;
	char **names;
} metadata;


// tree.h
typedef struct tree {
	int *leaf;
	int n;
	int *parent;
	int *child;
	int *group;
	char **name;

	int groups;
	int *group_size;
	int *group_offset;
} tree;


// activations.h
typedef enum {
	LOGISTIC, RELU, RELU6, RELIE, LINEAR, RAMP, TANH, PLSE, REVLEAKY, LEAKY, ELU, LOGGY, STAIR, HARDTAN, LHTAN, SELU, GELU, SWISH, MISH, HARD_MISH, NORM_CHAN, NORM_CHAN_SOFTMAX, NORM_CHAN_SOFTMAX_MAXVAL
}ACTIVATION;

// parser.h
typedef enum {
	IOU, GIOU, MSE, DIOU, CIOU
} IOU_LOSS;

// parser.h
typedef enum {
	DEFAULT_NMS, GREEDY_NMS, DIOU_NMS, CORNERS_NMS
} NMS_KIND;

// parser.h
typedef enum {
	YOLO_CENTER = 1 << 0, YOLO_LEFT_TOP = 1 << 1, YOLO_RIGHT_BOTTOM = 1 << 2
} YOLO_POINT;

// parser.h
typedef enum {
	NO_WEIGHTS, PER_FEATURE, PER_CHANNEL
} WEIGHTS_TYPE_T;

// parser.h
typedef enum {
	NO_NORMALIZATION, RELU_NORMALIZATION, SOFTMAX_NORMALIZATION
} WEIGHTS_NORMALIZATION_T;

// image.h
typedef enum{
	PNG,	// supported
	BMP,	// not supported (will be saved as JPG)
	TGA,	// not supported (will be saved as JPG)
	JPG		// supported
} IMTYPE;

// activations.h
typedef enum{
	MULT, ADD, SUB, DIV
} BINARY_ACTIVATION;

// blas.h
typedef struct contrastive_params {
	float sim;
	float exp_sim;
	float P;
	int i, j;
	int time_step_i, time_step_j;
} contrastive_params;


// layer.h
typedef enum {
	CONVOLUTIONAL,
	DECONVOLUTIONAL,
	CONNECTED,
	MAXPOOL,
	LOCAL_AVGPOOL,
	SOFTMAX,
	DETECTION,
	DROPOUT,
	CROP,
	ROUTE,
	COST,
	NORMALIZATION,
	AVGPOOL,
	LOCAL,
	SHORTCUT,
	SCALE_CHANNELS,
	SAM,
	ACTIVE,
	RNN,
	GRU,
	LSTM,
	CONV_LSTM,
	HISTORY,
	CRNN,
	BATCHNORM,
	NETWORK,
	XNOR,
	REGION,
	YOLO,
	GAUSSIAN_YOLO,
	ISEG,
	REORG,
	REORG_OLD,
	UPSAMPLE,
	LOGXENT,
	L2NORM,
	EMPTY,
	BLANK,
	CONTRASTIVE,
	IMPLICIT
} LAYER_TYPE;

// layer.h
typedef enum{
	SSE, MASKED, L1, SEG, SMOOTH,WGAN
} COST_TYPE;

// layer.h
typedef struct update_args {
	int batch;
	float learning_rate;
	float momentum;
	float decay;
	int adam;
	float B1;
	float B2;
	float eps;
	int t;
} update_args;

// layer.h
struct layer {
	LAYER_TYPE type; ///< @see @ref get_layer_string()
	ACTIVATION activation;
	ACTIVATION lstm_activation;
	COST_TYPE cost_type;
	void(*forward)   (struct layer, struct network_state);
	void(*backward)  (struct layer, struct network_state);
	void(*update)    (struct layer, int, float, float, float);
	void(*forward_gpu)   (struct layer, struct network_state);
	void(*backward_gpu)  (struct layer, struct network_state);
	void(*update_gpu)    (struct layer, int, float, float, float, float);
	layer *share_layer;
	int train;
	int avgpool;
	int batch_normalize;
	int shortcut;
	int batch;
	int dynamic_minibatch;
	int forced;
	int flipped;
	int inputs;
	int outputs;
	float mean_alpha;
	int nweights;
	int nbiases;
	int extra;
	int truths;
	int h, w, c;
	int out_h, out_w, out_c;
	int n;
	int max_boxes;
	int truth_size;
	int groups;
	int group_id;
	int size;
	int side;
	int stride;
	int stride_x;
	int stride_y;
	int dilation;
	int antialiasing;
	int maxpool_depth;
	int maxpool_zero_nonmax;
	int out_channels;
	float reverse;
	int coordconv;
	int flatten;
	int spatial;
	int pad;
	int sqrt;
	int flip;
	int index;
	int scale_wh;
	int binary;
	int xnor;
	int peephole;
	int use_bin_output;
	int keep_delta_gpu;
	int optimized_memory;
	int steps;
	int history_size;
	int bottleneck;
	float time_normalizer;
	int state_constrain;
	int hidden;
	int truth;
	float smooth;
	float dot;
	int deform;
	int grad_centr;
	int sway;
	int rotate;
	int stretch;
	int stretch_sway;
	float angle;
	float jitter;
	float resize;
	float saturation;
	float exposure;
	float shift;
	float ratio;
	float learning_rate_scale;
	float clip;
	int focal_loss;
	float *classes_multipliers;
	float label_smooth_eps;
	int noloss;
	int softmax;
	int classes;
	int detection;
	int embedding_layer_id;
	float *embedding_output;
	int embedding_size;
	float sim_thresh;
	int track_history_size;
	int dets_for_track;
	int dets_for_show;
	float track_ciou_norm;
	int coords;
	int background;
	int rescore;
	int objectness;
	int does_cost;
	int joint;
	int noadjust;
	int reorg;
	int log;
	int tanh;
	int *mask;
	int total;
	float bflops;

	int adam;
	float B1;
	float B2;
	float eps;

	int t;

	float alpha;
	float beta;
	float kappa;

	float coord_scale;
	float object_scale;
	float noobject_scale;
	float mask_scale;
	float class_scale;
	int bias_match;
	float random;
	float ignore_thresh;
	float truth_thresh;
	float iou_thresh;
	float thresh;
	float focus;
	int classfix;
	int absolute;
	int assisted_excitation;

	int onlyforward;
	int stopbackward;
	int train_only_bn;
	int dont_update;
	int burnin_update;
	int dontload;
	int dontsave;
	int dontloadscales;
	int numload;

	float temperature;
	float probability;
	float dropblock_size_rel;
	int dropblock_size_abs;
	int dropblock;
	float scale;

	int receptive_w;
	int receptive_h;
	int receptive_w_scale;
	int receptive_h_scale;

	char  * cweights;
	int   * indexes;
	int   * input_layers;
	int   * input_sizes;
	float **layers_output;
	float **layers_delta;
	WEIGHTS_TYPE_T weights_type;
	WEIGHTS_NORMALIZATION_T weights_normalization;
	int   * map;
	int   * counts;
	float ** sums;
	float * rand;
	float * cost;
	int *labels;
	int *class_ids;
	int contrastive_neg_max;
	float *cos_sim;
	float *exp_cos_sim;
	float *p_constrastive;
	contrastive_params *contrast_p_gpu;
	float * state;
	float * prev_state;
	float * forgot_state;
	float * forgot_delta;
	float * state_delta;
	float * combine_cpu;
	float * combine_delta_cpu;

	float *concat;
	float *concat_delta;

	float *binary_weights;

	float *biases;
	float *bias_updates;

	float *scales;
	float *scale_updates;

	float *weights_ema;
	float *biases_ema;
	float *scales_ema;

	float *weights;
	float *weight_updates;

	float scale_x_y;
	int objectness_smooth;
	int new_coords;
	int show_details;
	float max_delta;
	float uc_normalizer;
	float iou_normalizer;
	float obj_normalizer;
	float cls_normalizer;
	float delta_normalizer;
	IOU_LOSS iou_loss;
	IOU_LOSS iou_thresh_kind;
	NMS_KIND nms_kind;
	float beta_nms;
	YOLO_POINT yolo_point;

	char *align_bit_weights_gpu;
	float *mean_arr_gpu;
	float *align_workspace_gpu;
	float *transposed_align_workspace_gpu;
	int align_workspace_size;

	char *align_bit_weights;
	float *mean_arr;
	int align_bit_weights_size;
	int lda_align;
	int new_lda;
	int bit_align;

	float *col_image;
	float * delta;
	float * output;
	float * activation_input;
	int delta_pinned;
	int output_pinned;
	float * loss;
	float * squared;
	float * norms;

	float * spatial_mean;
	float * mean;
	float * variance;

	float * mean_delta;
	float * variance_delta;

	float * rolling_mean;
	float * rolling_variance;

	float * x;
	float * x_norm;

	float * m;
	float * v;

	float * bias_m;
	float * bias_v;
	float * scale_m;
	float * scale_v;


	float *z_cpu;
	float *r_cpu;
	float *h_cpu;
	float *stored_h_cpu;
	float * prev_state_cpu;

	float *temp_cpu;
	float *temp2_cpu;
	float *temp3_cpu;

	float *dh_cpu;
	float *hh_cpu;
	float *prev_cell_cpu;
	float *cell_cpu;
	float *f_cpu;
	float *i_cpu;
	float *g_cpu;
	float *o_cpu;
	float *c_cpu;
	float *stored_c_cpu;
	float *dc_cpu;

	float *binary_input;
	uint32_t *bin_re_packed_input;
	char *t_bit_input;

	struct layer *input_layer;
	struct layer *self_layer;
	struct layer *output_layer;

	struct layer *reset_layer;
	struct layer *update_layer;
	struct layer *state_layer;

	struct layer *input_gate_layer;
	struct layer *state_gate_layer;
	struct layer *input_save_layer;
	struct layer *state_save_layer;
	struct layer *input_state_layer;
	struct layer *state_state_layer;

	struct layer *input_z_layer;
	struct layer *state_z_layer;

	struct layer *input_r_layer;
	struct layer *state_r_layer;

	struct layer *input_h_layer;
	struct layer *state_h_layer;

	struct layer *wz;
	struct layer *uz;
	struct layer *wr;
	struct layer *ur;
	struct layer *wh;
	struct layer *uh;
	struct layer *uo;
	struct layer *wo;
	struct layer *vo;
	struct layer *uf;
	struct layer *wf;
	struct layer *vf;
	struct layer *ui;
	struct layer *wi;
	struct layer *vi;
	struct layer *ug;
	struct layer *wg;

	tree *softmax_tree;

	size_t workspace_size;

//#ifdef GPU
	int *indexes_gpu;

	int stream;
	int wait_stream_id;

	float *z_gpu;
	float *r_gpu;
	float *h_gpu;
	float *stored_h_gpu;
	float *bottelneck_hi_gpu;
	float *bottelneck_delta_gpu;

	float *temp_gpu;
	float *temp2_gpu;
	float *temp3_gpu;

	float *dh_gpu;
	float *hh_gpu;
	float *prev_cell_gpu;
	float *prev_state_gpu;
	float *last_prev_state_gpu;
	float *last_prev_cell_gpu;
	float *cell_gpu;
	float *f_gpu;
	float *i_gpu;
	float *g_gpu;
	float *o_gpu;
	float *c_gpu;
	float *stored_c_gpu;
	float *dc_gpu;

	// adam
	float *m_gpu;
	float *v_gpu;
	float *bias_m_gpu;
	float *scale_m_gpu;
	float *bias_v_gpu;
	float *scale_v_gpu;

	float * combine_gpu;
	float * combine_delta_gpu;

	float * forgot_state_gpu;
	float * forgot_delta_gpu;
	float * state_gpu;
	float * state_delta_gpu;
	float * gate_gpu;
	float * gate_delta_gpu;
	float * save_gpu;
	float * save_delta_gpu;
	float * concat_gpu;
	float * concat_delta_gpu;

	float *binary_input_gpu;
	float *binary_weights_gpu;
	float *bin_conv_shortcut_in_gpu;
	float *bin_conv_shortcut_out_gpu;

	float * mean_gpu;
	float * variance_gpu;
	float * m_cbn_avg_gpu;
	float * v_cbn_avg_gpu;

	float * rolling_mean_gpu;
	float * rolling_variance_gpu;

	float * variance_delta_gpu;
	float * mean_delta_gpu;

	float * col_image_gpu;

	float * x_gpu;
	float * x_norm_gpu;
	float * weights_gpu;
	float * weight_updates_gpu;
	float * weight_deform_gpu;
	float * weight_change_gpu;

	float * weights_gpu16;
	float * weight_updates_gpu16;

	float * biases_gpu;
	float * bias_updates_gpu;
	float * bias_change_gpu;

	float * scales_gpu;
	float * scale_updates_gpu;
	float * scale_change_gpu;

	float * input_antialiasing_gpu;
	float * output_gpu;
	float * output_avg_gpu;
	float * activation_input_gpu;
	float * loss_gpu;
	float * delta_gpu;
	float * cos_sim_gpu;
	float * rand_gpu;
	float * drop_blocks_scale;
	float * drop_blocks_scale_gpu;
	float * squared_gpu;
	float * norms_gpu;

	float *gt_gpu;
	float *a_avg_gpu;

	int *input_sizes_gpu;
	float **layers_output_gpu;
	float **layers_delta_gpu;
#ifdef CUDNN
	cudnnTensorDescriptor_t srcTensorDesc, dstTensorDesc;
	cudnnTensorDescriptor_t srcTensorDesc16, dstTensorDesc16;
	cudnnTensorDescriptor_t dsrcTensorDesc, ddstTensorDesc;
	cudnnTensorDescriptor_t dsrcTensorDesc16, ddstTensorDesc16;
	cudnnTensorDescriptor_t normTensorDesc, normDstTensorDesc, normDstTensorDescF16;
	cudnnFilterDescriptor_t weightDesc, weightDesc16;
	cudnnFilterDescriptor_t dweightDesc, dweightDesc16;
	cudnnConvolutionDescriptor_t convDesc;
	cudnnConvolutionFwdAlgo_t fw_algo, fw_algo16;
	cudnnConvolutionBwdDataAlgo_t bd_algo, bd_algo16;
	cudnnConvolutionBwdFilterAlgo_t bf_algo, bf_algo16;
	cudnnPoolingDescriptor_t poolingDesc;
#else   // CUDNN
	void* srcTensorDesc, *dstTensorDesc;
	void* srcTensorDesc16, *dstTensorDesc16;
	void* dsrcTensorDesc, *ddstTensorDesc;
	void* dsrcTensorDesc16, *ddstTensorDesc16;
	void* normTensorDesc, *normDstTensorDesc, *normDstTensorDescF16;
	void* weightDesc, *weightDesc16;
	void* dweightDesc, *dweightDesc16;
	void* convDesc;
	UNUSED_ENUM_TYPE fw_algo, fw_algo16;
	UNUSED_ENUM_TYPE bd_algo, bd_algo16;
	UNUSED_ENUM_TYPE bf_algo, bf_algo16;
	void* poolingDesc;
#endif  // CUDNN
//#endif  // GPU
};


// network.h
typedef enum {
	CONSTANT, STEP, EXP, POLY, STEPS, SIG, RANDOM, SGDR
} learning_rate_policy;

// network.h
typedef struct network {
	int n;
	int batch;
	uint64_t *seen;
	float *badlabels_reject_threshold;
	float *delta_rolling_max;
	float *delta_rolling_avg;
	float *delta_rolling_std;
	int weights_reject_freq;
	int equidistant_point;
	float badlabels_rejection_percentage;
	float num_sigmas_reject_badlabels;
	float ema_alpha;
	int *cur_iteration;
	float loss_scale;
	int *t;
	float epoch;
	int subdivisions;
	layer *layers;
	float *output;
	learning_rate_policy policy;
	int benchmark_layers;
	int *total_bbox;
	int *rewritten_bbox;

	float learning_rate;
	float learning_rate_min;
	float learning_rate_max;
	int batches_per_cycle;
	int batches_cycle_mult;
	float momentum;
	float decay;
	float gamma;
	float scale;
	float power;
	int time_steps;
	int step;
	int max_batches;
	int num_boxes;
	int train_images_num;
	float *seq_scales;
	float *scales;
	int   *steps;
	int num_steps;
	int burn_in;
	int cudnn_half;

	int adam;
	float B1;
	float B2;
	float eps;

	int inputs;
	int outputs;
	int truths;
	int notruth;
	/// The height of the network.  Must be divisible by @p 32.  E.g, @p 480.
	int h;
	/// The width of the network.  Must be divisible by @p 32.  E.g., @p 640.
	int w;
	/// The number of channels for the network.  Typically @p 3 when working with RGB images.
	int c;
	int max_crop;
	int min_crop;
	float max_ratio;
	float min_ratio;
	int center;
	int flip; ///< horizontal flip 50% probability augmentaiont for classifier training (default = 1)
	int gaussian_noise;
	int blur;
	int mixup;
	float label_smooth_eps;
	int resize_step;
	int attention;
	int adversarial;
	float adversarial_lr;
	float max_chart_loss;
	int letter_box;
	int mosaic_bound;
	int contrastive;
	int contrastive_jit_flip;
	int contrastive_color;
	int unsupervised;
	float angle;
	float aspect;
	float exposure;
	float saturation;
	float hue;
	int random;
	int track;
	int augment_speed;
	int sequential_subdivisions;
	int init_sequential_subdivisions;
	int current_subdivision;
	int try_fix_nan;

	int gpu_index;
	tree *hierarchy;

	float *input;
	float *truth;
	float *delta;
	float *workspace;
	int train;
	int index;
	float *cost;
	float clip;

//#ifdef GPU
	//float *input_gpu;
	//float *truth_gpu;
	float *delta_gpu;
	float *output_gpu;

	float *input_state_gpu;
	float *input_pinned_cpu;	///< memory allocated using @p cudaHostAlloc() which is used to transfer between the GPU and CPU
	int input_pinned_cpu_flag;

	float **input_gpu;
	float **truth_gpu;
	float **input16_gpu;
	float **output16_gpu;
	size_t *max_input16_size;
	size_t *max_output16_size;
	int wait_stream;

	void *cuda_graph;
	void *cuda_graph_exec;
	int use_cuda_graph;
	int *cuda_graph_ready;

	float *global_delta_gpu;
	float *state_delta_gpu;
	size_t max_delta_gpu_size;
//#endif  // GPU
	int optimized_memory;
	int dynamic_minibatch;
	size_t workspace_size_limit;
} network;

// network.h
typedef struct network_state {
	float *truth;
	float *input;
	float *delta;
	float *workspace;
	int train;
	int index;
	network net;
} network_state;

// image.h
typedef struct image {
	int w;
	int h;
	int c;
	float *data;
} image;

// box.h
typedef struct box {
	float x, y, w, h;
} box;

// box.h
typedef struct boxabs {
	float left, right, top, bot;
} boxabs;

// box.h
typedef struct dxrep {
	float dt, db, dl, dr;
} dxrep;

// box.h
typedef struct ious {
	float iou, giou, diou, ciou;
	dxrep dx_iou;
	dxrep dx_giou;
} ious;


// box.h
typedef struct detection{
	box bbox;
	int classes;
	int best_class_idx;
	float *prob;
	float *mask;
	float objectness;
	int sort_class;
	float *uc; ///< Gaussian_YOLOv3 - tx,ty,tw,th uncertainty
	int points; ///< bit-0 - center, bit-1 - top-left-corner, bit-2 - bottom-right-corner
	float *embeddings;  ///< embeddings for tracking
	int embedding_size;
	float sim;
	int track_id;
} detection;

// network.c -batch inference
typedef struct det_num_pair {
	int num;
	detection *dets;
} det_num_pair, *pdet_num_pair;

// matrix.h
typedef struct matrix
{
	int rows;
	int cols;
	float **vals;
} matrix;

// data.h
typedef struct data {
	int w, h;
	matrix X;
	matrix y;
	int shallow;
	int *num_boxes;
	box **boxes;
} data;


/** Things that we can do on a secondary thread.
* @see @ref load_thread()
* @see @ref load_args.type
*/
typedef enum
{
	CLASSIFICATION_DATA,
	DETECTION_DATA,
//	CAPTCHA_DATA, unused
	REGION_DATA,
	IMAGE_DATA, ///< causes @ref load_image() and @ref resize_image() to be called
	COMPARE_DATA,
	WRITING_DATA,
//	SWAG_DATA, unused
	TAG_DATA,
	OLD_CLASSIFICATION_DATA,
//	STUDY_DATA, unused
//	DET_DATA, unused
	SUPER_DATA,
	LETTERBOX_DATA,
//	REGRESSION_DATA, unused
//	SEGMENTATION_DATA, unused
//	INSTANCE_DATA, unused
//	ISEG_DATA unused
} data_type;


/** Used when a secondary thread is created to load things, such as images.
* @see @ref load_image()
* @see @ref data_type
*/
typedef struct load_args {
	int threads;
	char **paths;
	char *path;
	int n;
	int m;
	char **labels;
	int h;
	int w;
	int c;	///< Number of channels, typically 3 for RGB
	int out_w;
	int out_h;
	int nh;
	int nw;
	int num_boxes;
	int truth_size;
	int min, max, size;
	int classes;
	int background;
	int scale;
	int center;
	int coords;
	int mini_batch;
	int track;
	int augment_speed;
	int letter_box;
	int mosaic_bound;
	int show_imgs;
	int contrastive;
	int contrastive_jit_flip;
	int contrastive_color;
	float jitter;
	float resize;
	int flip;
	int gaussian_noise;
	int blur;
	int mixup;
	float label_smooth_eps;
	float angle;
	float aspect;
	float saturation;
	float exposure;
	float hue;
	data *d;
	image *im;
	image *resized;
	data_type type;
	tree *hierarchy;
} load_args;


// data.h
typedef struct box_label {
	int id;
	int track_id;
	float x, y, w, h;
	float left, right, top, bottom;
} box_label;

// -----------------------------------------------------


// parser.c
network *load_network(char *cfg, char *weights, int clear);
network *load_network_custom(char *cfg, char *weights, int clear, int batch);
void free_network(network net);
void free_network_ptr(network* net);

// network.c
load_args get_base_args(network *net);

// box.h
void do_nms_sort(detection *dets, int total, int classes, float thresh);
void do_nms_obj(detection *dets, int total, int classes, float thresh);
void diounms_sort(detection *dets, int total, int classes, float thresh, NMS_KIND nms_kind, float beta1);

// network.h
float *network_predict(network net, float *input);
float *network_predict_ptr(network *net, float *input);
detection *get_network_boxes(network *net, int w, int h, float thresh, float hier, int *map, int relative, int *num, int letter);
det_num_pair* network_predict_batch(network *net, image im, int batch_size, int w, int h, float thresh, float hier, int *map, int relative, int letter);
void free_detections(detection *dets, int n);
void free_batch_detections(det_num_pair *det_num_pairs, int n);
void fuse_conv_batchnorm(network net);
void calculate_binary_weights(network net);
char *detection_to_json(detection *dets, int nboxes, int classes, char **names, long long int frame_id, char *filename);

layer* get_network_layer(network* net, int i);
detection *make_network_boxes(network *net, float thresh, int *num);
void reset_rnn(network *net);
float *network_predict_image(network *net, image im);
float *network_predict_image_letterbox(network *net, image im);
float validate_detector_map(char *datacfg, char *cfgfile, char *weightfile, float thresh_calc_avg_iou, const float iou_thresh, const int map_points, int letter_box, network *existing_net);
void train_detector(char *datacfg, char *cfgfile, char *weightfile, int *gpus, int ngpus, int clear, int dont_show, int calc_map, float thresh, float iou_thresh, int mjpeg_port, int show_imgs, int benchmark_layers, char* chart_path);
void test_detector(char *datacfg, char *cfgfile, char *weightfile, char *filename, float thresh, float hier_thresh, int dont_show, int ext_output, int save_labels, char *outfile, int letter_box, int benchmark_layers);
int network_width(network *net);
int network_height(network *net);
void optimize_picture(network *net, image orig, int max_layer, float scale, float rate, float thresh, int norm);

// image.h
void make_image_red(image im);
image make_attention_image(int img_size, float *original_delta_cpu, float *original_input_cpu, int w, int h, int c, float alpha);
image resize_image(image im, int w, int h);
void quantize_image(image im);
void copy_image_from_bytes(image im, char *pdata);
image letterbox_image(image im, int w, int h);
void rgbgr_image(image im);
image make_image(int w, int h, int c);
image load_image_color(char *filename, int w, int h);
void free_image(image m);
image crop_image(image im, int dx, int dy, int w, int h);
image resize_min(image im, int min);

// layer.h
void free_layer_custom(layer l, int keep_cudnn_desc);
void free_layer(layer l);

// data.c
void free_data(data d);
pthread_t load_data(load_args args);
void free_load_threads(void *ptr);
pthread_t load_data_in_thread(load_args args);
void *load_thread(void *ptr);

// dark_cuda.h
void cuda_pull_array(float *x_gpu, float *x, size_t n);
void cuda_pull_array_async(float *x_gpu, float *x, size_t n);
void cuda_set_device(int n);
void *cuda_get_context();

// utils.h
void free_ptrs(void **ptrs, int n);
void top_k(float *a, int n, int k, int *index);

// tree.h
tree *read_tree(char *filename);

// option_list.h
metadata get_metadata(char *file);


// http_stream.h
void delete_json_sender();
void send_json_custom(char const* send_buf, int port, int timeout);
double get_time_point();
void start_timer();
void stop_timer();
double get_time();
void stop_timer_and_show();
void stop_timer_and_show_name(char *name);
void show_total_time();

void set_track_id(detection *new_dets, int new_dets_num, float thresh, float sim_thresh, float track_ciou_norm, int deque_size, int dets_for_track, int dets_for_show);
int fill_remaining_id(detection *new_dets, int new_dets_num, int new_track_id, float thresh);


// gemm.h
void init_cpu();

#ifdef __cplusplus
}
#endif  // __cplusplus


