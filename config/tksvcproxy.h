/*
 *----------------------------------------------------------------------
 *    micro T-Kernel
 *
 *    Copyright (C) 2006-2011 by Ken Sakamura. All rights reserved.
 *    micro T-Kernel is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Version:   1.01.01
 *    Released by T-Engine Forum(http://www.t-engine.org) at 2011/12/12.
 *
 *----------------------------------------------------------------------
 */

/*
 *	tksvcproxy.h
 *	micro T-Kernel system call proxy for direct call
 */

#define tk_cre_tsk_impl	tk_cre_tsk
#define tk_del_tsk_impl	tk_del_tsk
#define tk_sta_tsk_impl	tk_sta_tsk
#define tk_ext_tsk_impl	tk_ext_tsk
#define tk_exd_tsk_impl	tk_exd_tsk
#define tk_ter_tsk_impl	tk_ter_tsk
#define tk_dis_dsp_impl	tk_dis_dsp
#define tk_ena_dsp_impl	tk_ena_dsp
#define tk_chg_pri_impl	tk_chg_pri
#define tk_rot_rdq_impl	tk_rot_rdq
#define tk_rel_wai_impl	tk_rel_wai
#define tk_get_tid_impl	tk_get_tid
#define tk_get_reg_impl	tk_get_reg
#define tk_set_reg_impl	tk_set_reg
#define tk_ref_tsk_impl	tk_ref_tsk
#define tk_sus_tsk_impl	tk_sus_tsk
#define tk_rsm_tsk_impl	tk_rsm_tsk
#define tk_frsm_tsk_impl	tk_frsm_tsk
#define tk_slp_tsk_impl	tk_slp_tsk
#define tk_wup_tsk_impl	tk_wup_tsk
#define tk_can_wup_impl	tk_can_wup
#define tk_cre_sem_impl	tk_cre_sem
#define tk_del_sem_impl	tk_del_sem
#define tk_sig_sem_impl	tk_sig_sem
#define tk_wai_sem_impl	tk_wai_sem
#define tk_ref_sem_impl	tk_ref_sem
#define tk_cre_mtx_impl	tk_cre_mtx
#define tk_del_mtx_impl	tk_del_mtx
#define tk_loc_mtx_impl	tk_loc_mtx
#define tk_unl_mtx_impl	tk_unl_mtx
#define tk_ref_mtx_impl	tk_ref_mtx
#define tk_cre_flg_impl	tk_cre_flg
#define tk_del_flg_impl	tk_del_flg
#define tk_set_flg_impl	tk_set_flg
#define tk_clr_flg_impl	tk_clr_flg
#define tk_wai_flg_impl	tk_wai_flg
#define tk_ref_flg_impl	tk_ref_flg
#define tk_cre_mbx_impl	tk_cre_mbx
#define tk_del_mbx_impl	tk_del_mbx
#define tk_snd_mbx_impl	tk_snd_mbx
#define tk_rcv_mbx_impl	tk_rcv_mbx
#define tk_ref_mbx_impl	tk_ref_mbx
#define tk_cre_mbf_impl	tk_cre_mbf
#define tk_del_mbf_impl	tk_del_mbf
#define tk_snd_mbf_impl	tk_snd_mbf
#define tk_rcv_mbf_impl	tk_rcv_mbf
#define tk_ref_mbf_impl	tk_ref_mbf
#define tk_cre_por_impl	tk_cre_por
#define tk_del_por_impl	tk_del_por
#define tk_cal_por_impl	tk_cal_por
#define tk_acp_por_impl	tk_acp_por
#define tk_fwd_por_impl	tk_fwd_por
#define tk_rpl_rdv_impl	tk_rpl_rdv
#define tk_ref_por_impl	tk_ref_por
#define tk_def_int_impl	tk_def_int
#define tk_ret_int_impl	tk_ret_int
#define tk_cre_mpl_impl	tk_cre_mpl
#define tk_del_mpl_impl	tk_del_mpl
#define tk_get_mpl_impl	tk_get_mpl
#define tk_rel_mpl_impl	tk_rel_mpl
#define tk_ref_mpl_impl	tk_ref_mpl
#define tk_cre_mpf_impl	tk_cre_mpf
#define tk_del_mpf_impl	tk_del_mpf
#define tk_get_mpf_impl	tk_get_mpf
#define tk_rel_mpf_impl	tk_rel_mpf
#define tk_ref_mpf_impl	tk_ref_mpf
#define tk_set_tim_impl	tk_set_tim
#define tk_get_tim_impl	tk_get_tim
#define tk_get_otm_impl	tk_get_otm
#define tk_dly_tsk_impl	tk_dly_tsk
#define tk_cre_cyc_impl	tk_cre_cyc
#define tk_del_cyc_impl	tk_del_cyc
#define tk_sta_cyc_impl	tk_sta_cyc
#define tk_stp_cyc_impl	tk_stp_cyc
#define tk_ref_cyc_impl	tk_ref_cyc
#define tk_cre_alm_impl	tk_cre_alm
#define tk_del_alm_impl	tk_del_alm
#define tk_sta_alm_impl	tk_sta_alm
#define tk_stp_alm_impl	tk_stp_alm
#define tk_ref_alm_impl	tk_ref_alm
#define tk_ref_ver_impl	tk_ref_ver
#define tk_ref_sys_impl	tk_ref_sys
#define tk_def_ssy_impl	tk_def_ssy
#define tk_ref_ssy_impl	tk_ref_ssy
#define tk_opn_dev_impl	tk_opn_dev
#define tk_cls_dev_impl	tk_cls_dev
#define tk_rea_dev_impl	tk_rea_dev
#define tk_srea_dev_impl	tk_srea_dev
#define tk_wri_dev_impl	tk_wri_dev
#define tk_swri_dev_impl	tk_swri_dev
#define tk_wai_dev_impl	tk_wai_dev
#define tk_sus_dev_impl	tk_sus_dev
#define tk_get_dev_impl	tk_get_dev
#define tk_ref_dev_impl	tk_ref_dev
#define tk_oref_dev_impl	tk_oref_dev
#define tk_lst_dev_impl	tk_lst_dev
#define tk_evt_dev_impl	tk_evt_dev
#define tk_def_dev_impl	tk_def_dev
#define tk_ref_idv_impl	tk_ref_idv
