#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                       Code generated with SymPy 1.12                       *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_545739425295966695) {
   out_545739425295966695[0] = delta_x[0] + nom_x[0];
   out_545739425295966695[1] = delta_x[1] + nom_x[1];
   out_545739425295966695[2] = delta_x[2] + nom_x[2];
   out_545739425295966695[3] = delta_x[3] + nom_x[3];
   out_545739425295966695[4] = delta_x[4] + nom_x[4];
   out_545739425295966695[5] = delta_x[5] + nom_x[5];
   out_545739425295966695[6] = delta_x[6] + nom_x[6];
   out_545739425295966695[7] = delta_x[7] + nom_x[7];
   out_545739425295966695[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_4195167441435896958) {
   out_4195167441435896958[0] = -nom_x[0] + true_x[0];
   out_4195167441435896958[1] = -nom_x[1] + true_x[1];
   out_4195167441435896958[2] = -nom_x[2] + true_x[2];
   out_4195167441435896958[3] = -nom_x[3] + true_x[3];
   out_4195167441435896958[4] = -nom_x[4] + true_x[4];
   out_4195167441435896958[5] = -nom_x[5] + true_x[5];
   out_4195167441435896958[6] = -nom_x[6] + true_x[6];
   out_4195167441435896958[7] = -nom_x[7] + true_x[7];
   out_4195167441435896958[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_8644881507184810358) {
   out_8644881507184810358[0] = 1.0;
   out_8644881507184810358[1] = 0;
   out_8644881507184810358[2] = 0;
   out_8644881507184810358[3] = 0;
   out_8644881507184810358[4] = 0;
   out_8644881507184810358[5] = 0;
   out_8644881507184810358[6] = 0;
   out_8644881507184810358[7] = 0;
   out_8644881507184810358[8] = 0;
   out_8644881507184810358[9] = 0;
   out_8644881507184810358[10] = 1.0;
   out_8644881507184810358[11] = 0;
   out_8644881507184810358[12] = 0;
   out_8644881507184810358[13] = 0;
   out_8644881507184810358[14] = 0;
   out_8644881507184810358[15] = 0;
   out_8644881507184810358[16] = 0;
   out_8644881507184810358[17] = 0;
   out_8644881507184810358[18] = 0;
   out_8644881507184810358[19] = 0;
   out_8644881507184810358[20] = 1.0;
   out_8644881507184810358[21] = 0;
   out_8644881507184810358[22] = 0;
   out_8644881507184810358[23] = 0;
   out_8644881507184810358[24] = 0;
   out_8644881507184810358[25] = 0;
   out_8644881507184810358[26] = 0;
   out_8644881507184810358[27] = 0;
   out_8644881507184810358[28] = 0;
   out_8644881507184810358[29] = 0;
   out_8644881507184810358[30] = 1.0;
   out_8644881507184810358[31] = 0;
   out_8644881507184810358[32] = 0;
   out_8644881507184810358[33] = 0;
   out_8644881507184810358[34] = 0;
   out_8644881507184810358[35] = 0;
   out_8644881507184810358[36] = 0;
   out_8644881507184810358[37] = 0;
   out_8644881507184810358[38] = 0;
   out_8644881507184810358[39] = 0;
   out_8644881507184810358[40] = 1.0;
   out_8644881507184810358[41] = 0;
   out_8644881507184810358[42] = 0;
   out_8644881507184810358[43] = 0;
   out_8644881507184810358[44] = 0;
   out_8644881507184810358[45] = 0;
   out_8644881507184810358[46] = 0;
   out_8644881507184810358[47] = 0;
   out_8644881507184810358[48] = 0;
   out_8644881507184810358[49] = 0;
   out_8644881507184810358[50] = 1.0;
   out_8644881507184810358[51] = 0;
   out_8644881507184810358[52] = 0;
   out_8644881507184810358[53] = 0;
   out_8644881507184810358[54] = 0;
   out_8644881507184810358[55] = 0;
   out_8644881507184810358[56] = 0;
   out_8644881507184810358[57] = 0;
   out_8644881507184810358[58] = 0;
   out_8644881507184810358[59] = 0;
   out_8644881507184810358[60] = 1.0;
   out_8644881507184810358[61] = 0;
   out_8644881507184810358[62] = 0;
   out_8644881507184810358[63] = 0;
   out_8644881507184810358[64] = 0;
   out_8644881507184810358[65] = 0;
   out_8644881507184810358[66] = 0;
   out_8644881507184810358[67] = 0;
   out_8644881507184810358[68] = 0;
   out_8644881507184810358[69] = 0;
   out_8644881507184810358[70] = 1.0;
   out_8644881507184810358[71] = 0;
   out_8644881507184810358[72] = 0;
   out_8644881507184810358[73] = 0;
   out_8644881507184810358[74] = 0;
   out_8644881507184810358[75] = 0;
   out_8644881507184810358[76] = 0;
   out_8644881507184810358[77] = 0;
   out_8644881507184810358[78] = 0;
   out_8644881507184810358[79] = 0;
   out_8644881507184810358[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_3030064181539059414) {
   out_3030064181539059414[0] = state[0];
   out_3030064181539059414[1] = state[1];
   out_3030064181539059414[2] = state[2];
   out_3030064181539059414[3] = state[3];
   out_3030064181539059414[4] = state[4];
   out_3030064181539059414[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_3030064181539059414[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_3030064181539059414[7] = state[7];
   out_3030064181539059414[8] = state[8];
}
void F_fun(double *state, double dt, double *out_5649752907200933170) {
   out_5649752907200933170[0] = 1;
   out_5649752907200933170[1] = 0;
   out_5649752907200933170[2] = 0;
   out_5649752907200933170[3] = 0;
   out_5649752907200933170[4] = 0;
   out_5649752907200933170[5] = 0;
   out_5649752907200933170[6] = 0;
   out_5649752907200933170[7] = 0;
   out_5649752907200933170[8] = 0;
   out_5649752907200933170[9] = 0;
   out_5649752907200933170[10] = 1;
   out_5649752907200933170[11] = 0;
   out_5649752907200933170[12] = 0;
   out_5649752907200933170[13] = 0;
   out_5649752907200933170[14] = 0;
   out_5649752907200933170[15] = 0;
   out_5649752907200933170[16] = 0;
   out_5649752907200933170[17] = 0;
   out_5649752907200933170[18] = 0;
   out_5649752907200933170[19] = 0;
   out_5649752907200933170[20] = 1;
   out_5649752907200933170[21] = 0;
   out_5649752907200933170[22] = 0;
   out_5649752907200933170[23] = 0;
   out_5649752907200933170[24] = 0;
   out_5649752907200933170[25] = 0;
   out_5649752907200933170[26] = 0;
   out_5649752907200933170[27] = 0;
   out_5649752907200933170[28] = 0;
   out_5649752907200933170[29] = 0;
   out_5649752907200933170[30] = 1;
   out_5649752907200933170[31] = 0;
   out_5649752907200933170[32] = 0;
   out_5649752907200933170[33] = 0;
   out_5649752907200933170[34] = 0;
   out_5649752907200933170[35] = 0;
   out_5649752907200933170[36] = 0;
   out_5649752907200933170[37] = 0;
   out_5649752907200933170[38] = 0;
   out_5649752907200933170[39] = 0;
   out_5649752907200933170[40] = 1;
   out_5649752907200933170[41] = 0;
   out_5649752907200933170[42] = 0;
   out_5649752907200933170[43] = 0;
   out_5649752907200933170[44] = 0;
   out_5649752907200933170[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_5649752907200933170[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_5649752907200933170[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_5649752907200933170[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_5649752907200933170[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_5649752907200933170[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_5649752907200933170[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_5649752907200933170[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_5649752907200933170[53] = -9.8000000000000007*dt;
   out_5649752907200933170[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_5649752907200933170[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_5649752907200933170[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_5649752907200933170[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_5649752907200933170[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_5649752907200933170[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_5649752907200933170[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_5649752907200933170[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_5649752907200933170[62] = 0;
   out_5649752907200933170[63] = 0;
   out_5649752907200933170[64] = 0;
   out_5649752907200933170[65] = 0;
   out_5649752907200933170[66] = 0;
   out_5649752907200933170[67] = 0;
   out_5649752907200933170[68] = 0;
   out_5649752907200933170[69] = 0;
   out_5649752907200933170[70] = 1;
   out_5649752907200933170[71] = 0;
   out_5649752907200933170[72] = 0;
   out_5649752907200933170[73] = 0;
   out_5649752907200933170[74] = 0;
   out_5649752907200933170[75] = 0;
   out_5649752907200933170[76] = 0;
   out_5649752907200933170[77] = 0;
   out_5649752907200933170[78] = 0;
   out_5649752907200933170[79] = 0;
   out_5649752907200933170[80] = 1;
}
void h_25(double *state, double *unused, double *out_445506004756160554) {
   out_445506004756160554[0] = state[6];
}
void H_25(double *state, double *unused, double *out_4323680074794656357) {
   out_4323680074794656357[0] = 0;
   out_4323680074794656357[1] = 0;
   out_4323680074794656357[2] = 0;
   out_4323680074794656357[3] = 0;
   out_4323680074794656357[4] = 0;
   out_4323680074794656357[5] = 0;
   out_4323680074794656357[6] = 1;
   out_4323680074794656357[7] = 0;
   out_4323680074794656357[8] = 0;
}
void h_24(double *state, double *unused, double *out_4242429278414397240) {
   out_4242429278414397240[0] = state[4];
   out_4242429278414397240[1] = state[5];
}
void H_24(double *state, double *unused, double *out_8311678367892118294) {
   out_8311678367892118294[0] = 0;
   out_8311678367892118294[1] = 0;
   out_8311678367892118294[2] = 0;
   out_8311678367892118294[3] = 0;
   out_8311678367892118294[4] = 1;
   out_8311678367892118294[5] = 0;
   out_8311678367892118294[6] = 0;
   out_8311678367892118294[7] = 0;
   out_8311678367892118294[8] = 0;
   out_8311678367892118294[9] = 0;
   out_8311678367892118294[10] = 0;
   out_8311678367892118294[11] = 0;
   out_8311678367892118294[12] = 0;
   out_8311678367892118294[13] = 0;
   out_8311678367892118294[14] = 1;
   out_8311678367892118294[15] = 0;
   out_8311678367892118294[16] = 0;
   out_8311678367892118294[17] = 0;
}
void h_30(double *state, double *unused, double *out_3191943523497861796) {
   out_3191943523497861796[0] = state[4];
}
void H_30(double *state, double *unused, double *out_7206373657423278504) {
   out_7206373657423278504[0] = 0;
   out_7206373657423278504[1] = 0;
   out_7206373657423278504[2] = 0;
   out_7206373657423278504[3] = 0;
   out_7206373657423278504[4] = 1;
   out_7206373657423278504[5] = 0;
   out_7206373657423278504[6] = 0;
   out_7206373657423278504[7] = 0;
   out_7206373657423278504[8] = 0;
}
void h_26(double *state, double *unused, double *out_773197910430007770) {
   out_773197910430007770[0] = state[7];
}
void H_26(double *state, double *unused, double *out_582176755920600133) {
   out_582176755920600133[0] = 0;
   out_582176755920600133[1] = 0;
   out_582176755920600133[2] = 0;
   out_582176755920600133[3] = 0;
   out_582176755920600133[4] = 0;
   out_582176755920600133[5] = 0;
   out_582176755920600133[6] = 0;
   out_582176755920600133[7] = 1;
   out_582176755920600133[8] = 0;
}
void h_27(double *state, double *unused, double *out_4556167262904685091) {
   out_4556167262904685091[0] = state[3];
}
void H_27(double *state, double *unused, double *out_2019577815850991376) {
   out_2019577815850991376[0] = 0;
   out_2019577815850991376[1] = 0;
   out_2019577815850991376[2] = 0;
   out_2019577815850991376[3] = 1;
   out_2019577815850991376[4] = 0;
   out_2019577815850991376[5] = 0;
   out_2019577815850991376[6] = 0;
   out_2019577815850991376[7] = 0;
   out_2019577815850991376[8] = 0;
}
void h_29(double *state, double *unused, double *out_3207097993915987350) {
   out_3207097993915987350[0] = state[1];
}
void H_29(double *state, double *unused, double *out_4704572471965808471) {
   out_4704572471965808471[0] = 0;
   out_4704572471965808471[1] = 1;
   out_4704572471965808471[2] = 0;
   out_4704572471965808471[3] = 0;
   out_4704572471965808471[4] = 0;
   out_4704572471965808471[5] = 0;
   out_4704572471965808471[6] = 0;
   out_4704572471965808471[7] = 0;
   out_4704572471965808471[8] = 0;
}
void h_28(double *state, double *unused, double *out_2878976171524015722) {
   out_2878976171524015722[0] = state[0];
}
void H_28(double *state, double *unused, double *out_2269845360546766594) {
   out_2269845360546766594[0] = 1;
   out_2269845360546766594[1] = 0;
   out_2269845360546766594[2] = 0;
   out_2269845360546766594[3] = 0;
   out_2269845360546766594[4] = 0;
   out_2269845360546766594[5] = 0;
   out_2269845360546766594[6] = 0;
   out_2269845360546766594[7] = 0;
   out_2269845360546766594[8] = 0;
}
void h_31(double *state, double *unused, double *out_6325329221594190382) {
   out_6325329221594190382[0] = state[8];
}
void H_31(double *state, double *unused, double *out_4354326036671616785) {
   out_4354326036671616785[0] = 0;
   out_4354326036671616785[1] = 0;
   out_4354326036671616785[2] = 0;
   out_4354326036671616785[3] = 0;
   out_4354326036671616785[4] = 0;
   out_4354326036671616785[5] = 0;
   out_4354326036671616785[6] = 0;
   out_4354326036671616785[7] = 0;
   out_4354326036671616785[8] = 1;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_545739425295966695) {
  err_fun(nom_x, delta_x, out_545739425295966695);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_4195167441435896958) {
  inv_err_fun(nom_x, true_x, out_4195167441435896958);
}
void car_H_mod_fun(double *state, double *out_8644881507184810358) {
  H_mod_fun(state, out_8644881507184810358);
}
void car_f_fun(double *state, double dt, double *out_3030064181539059414) {
  f_fun(state,  dt, out_3030064181539059414);
}
void car_F_fun(double *state, double dt, double *out_5649752907200933170) {
  F_fun(state,  dt, out_5649752907200933170);
}
void car_h_25(double *state, double *unused, double *out_445506004756160554) {
  h_25(state, unused, out_445506004756160554);
}
void car_H_25(double *state, double *unused, double *out_4323680074794656357) {
  H_25(state, unused, out_4323680074794656357);
}
void car_h_24(double *state, double *unused, double *out_4242429278414397240) {
  h_24(state, unused, out_4242429278414397240);
}
void car_H_24(double *state, double *unused, double *out_8311678367892118294) {
  H_24(state, unused, out_8311678367892118294);
}
void car_h_30(double *state, double *unused, double *out_3191943523497861796) {
  h_30(state, unused, out_3191943523497861796);
}
void car_H_30(double *state, double *unused, double *out_7206373657423278504) {
  H_30(state, unused, out_7206373657423278504);
}
void car_h_26(double *state, double *unused, double *out_773197910430007770) {
  h_26(state, unused, out_773197910430007770);
}
void car_H_26(double *state, double *unused, double *out_582176755920600133) {
  H_26(state, unused, out_582176755920600133);
}
void car_h_27(double *state, double *unused, double *out_4556167262904685091) {
  h_27(state, unused, out_4556167262904685091);
}
void car_H_27(double *state, double *unused, double *out_2019577815850991376) {
  H_27(state, unused, out_2019577815850991376);
}
void car_h_29(double *state, double *unused, double *out_3207097993915987350) {
  h_29(state, unused, out_3207097993915987350);
}
void car_H_29(double *state, double *unused, double *out_4704572471965808471) {
  H_29(state, unused, out_4704572471965808471);
}
void car_h_28(double *state, double *unused, double *out_2878976171524015722) {
  h_28(state, unused, out_2878976171524015722);
}
void car_H_28(double *state, double *unused, double *out_2269845360546766594) {
  H_28(state, unused, out_2269845360546766594);
}
void car_h_31(double *state, double *unused, double *out_6325329221594190382) {
  h_31(state, unused, out_6325329221594190382);
}
void car_H_31(double *state, double *unused, double *out_4354326036671616785) {
  H_31(state, unused, out_4354326036671616785);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
