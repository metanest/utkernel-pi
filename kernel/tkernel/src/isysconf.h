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
 *	isysconf.h
 *	Default Value Definition for System Configuration File
 */

#ifndef _ISYSCONF_
#define _ISYSCONF_

/*
 * Definition of unused system call
 */
#if _Csym == 0

#ifndef USE_FUNC_TK_CRE_SEM
#define tk_cre_sem_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_SEM
#define tk_del_sem_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_SIG_SEM
#define tk_sig_sem_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_WAI_SEM
#define tk_wai_sem_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_SEM
#define tk_ref_sem_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_SEM
#define td_lst_sem_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_SEM
#define td_ref_sem_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_SEM_QUE
#define td_sem_que_impl	knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_FLG
#define tk_cre_flg_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_FLG
#define tk_del_flg_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_SET_FLG
#define tk_set_flg_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_CLR_FLG
#define tk_clr_flg_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_WAI_FLG
#define tk_wai_flg_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_FLG
#define tk_ref_flg_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_FLG
#define td_lst_flg_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_FLG
#define td_ref_flg_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_FLG_QUE
#define td_flg_que_impl	knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_MBX
#define tk_cre_mbx_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_MBX
#define tk_del_mbx_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_SND_MBX
#define tk_snd_mbx_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_RCV_MBX
#define tk_rcv_mbx_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_MBX
#define tk_ref_mbx_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_MBX
#define td_lst_mbx_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_MBX
#define td_ref_mbx_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_MBX_QUE
#define td_mbx_que_impl	knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_MBF
#define tk_cre_mbf_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_MBF
#define tk_del_mbf_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_SND_MBF
#define tk_snd_mbf_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_RCV_MBF
#define tk_rcv_mbf_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_MBF
#define tk_ref_mbf_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_MBF
#define td_lst_mbf_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_MBF
#define td_ref_mbf_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_SMBF_QUE
#define td_smbf_que_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_RMBF_QUE
#define td_rmbf_que_impl	knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_POR
#define tk_cre_por_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_POR
#define tk_del_por_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_CAL_POR
#define tk_cal_por_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_ACP_POR
#define tk_acp_por_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_FWD_POR
#define tk_fwd_por_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_RPL_RDV
#define tk_rpl_rdv_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_POR
#define tk_ref_por_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_POR
#define td_lst_por_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_POR
#define td_ref_por_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_ACP_QUE
#define td_acp_que_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_CAL_QUE
#define td_cal_que_impl	knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_MTX
#define tk_cre_mtx_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_MTX
#define tk_del_mtx_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_LOC_MTX
#define tk_loc_mtx_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_UNL_MTX
#define tk_unl_mtx_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_MTX
#define tk_ref_mtx_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_MTX
#define td_lst_mtx_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_MTX
#define td_ref_mtx_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_MTX_QUE
#define td_mtx_que_impl	knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_MPL
#define tk_cre_mpl_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_MPL
#define tk_del_mpl_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_GET_MPL
#define tk_get_mpl_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_REL_MPL
#define tk_rel_mpl_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_MPL
#define tk_ref_mpl_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_MPL
#define td_lst_mpl_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_MPL
#define td_ref_mpl_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_MPL_QUE
#define td_mpl_que_impl	knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_MPF
#define tk_cre_mpf_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_MPF
#define tk_del_mpf_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_GET_MPF
#define tk_get_mpf_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_REL_MPF
#define tk_rel_mpf_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_MPF
#define tk_ref_mpf_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_MPF
#define td_lst_mpf_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_MPF
#define td_ref_mpf_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_MPF_QUE
#define td_mpf_que_impl	knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_CYC
#define tk_cre_cyc_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_CYC
#define tk_del_cyc_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_STA_CYC
#define tk_sta_cyc_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_STP_CYC
#define tk_stp_cyc_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_CYC
#define tk_ref_cyc_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_CYC
#define td_lst_cyc_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_CYC
#define td_ref_cyc_impl	knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_ALM
#define tk_cre_alm_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_ALM
#define tk_del_alm_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_STA_ALM
#define tk_sta_alm_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_STP_ALM
#define tk_stp_alm_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_ALM
#define tk_ref_alm_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_ALM
#define td_lst_alm_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_ALM
#define td_ref_alm_impl	knl_no_support
#endif

#ifndef USE_FUNC_TK_DEF_SSY
#define tk_def_ssy_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_SSY
#define tk_ref_ssy_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_SSY
#define td_lst_ssy_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_SSY
#define td_ref_ssy_impl	knl_no_support
#endif

#ifndef USE_FUNC_TK_OPN_DEV
#define tk_opn_dev_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_CLS_DEV
#define tk_cls_dev_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_REA_DEV
#define tk_rea_dev_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_SREA_DEV
#define tk_srea_dev_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_WRI_DEV
#define tk_wri_dev_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_SWRI_DEV
#define tk_swri_dev_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_WAI_DEV
#define tk_wai_dev_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_SUS_DEV
#define tk_sus_dev_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_GET_DEV
#define tk_get_dev_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_DEV
#define tk_ref_dev_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_OREF_DEV
#define tk_oref_dev_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_LST_DEV
#define tk_lst_dev_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_EVT_DEV
#define tk_evt_dev_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_DEF_DEV
#define tk_def_dev_impl	knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_IDV
#define tk_ref_idv_impl	knl_no_support
#endif

#ifndef USE_FUNC_TD_HOK_SVC
#define td_hok_svc_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_HOK_DSP
#define td_hok_dsp_impl	knl_no_support
#endif
#ifndef USE_FUNC_TD_HOK_INT
#define td_hok_int_impl	knl_no_support
#endif

#else /* _Csym == 1 */

#ifndef USE_FUNC_TK_CRE_SEM
#define _tk_cre_sem_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_SEM
#define _tk_del_sem_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_SIG_SEM
#define _tk_sig_sem_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_WAI_SEM
#define _tk_wai_sem_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_SEM
#define _tk_ref_sem_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_SEM
#define _td_lst_sem_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_SEM
#define _td_ref_sem_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_SEM_QUE
#define _td_sem_que_impl	_knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_FLG
#define _tk_cre_flg_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_FLG
#define _tk_del_flg_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_SET_FLG
#define _tk_set_flg_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_CLR_FLG
#define _tk_clr_flg_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_WAI_FLG
#define _tk_wai_flg_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_FLG
#define _tk_ref_flg_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_FLG
#define _td_lst_flg_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_FLG
#define _td_ref_flg_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_FLG_QUE
#define _td_flg_que_impl	_knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_MBX
#define _tk_cre_mbx_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_MBX
#define _tk_del_mbx_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_SND_MBX
#define _tk_snd_mbx_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_RCV_MBX
#define _tk_rcv_mbx_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_MBX
#define _tk_ref_mbx_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_MBX
#define _td_lst_mbx_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_MBX
#define _td_ref_mbx_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_MBX_QUE
#define _td_mbx_que_impl	_knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_MBF
#define _tk_cre_mbf_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_MBF
#define _tk_del_mbf_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_SND_MBF
#define _tk_snd_mbf_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_RCV_MBF
#define _tk_rcv_mbf_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_MBF
#define _tk_ref_mbf_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_MBF
#define _td_lst_mbf_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_MBF
#define _td_ref_mbf_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_SMBF_QUE
#define _td_smbf_que_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_RMBF_QUE
#define _td_rmbf_que_impl	_knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_POR
#define _tk_cre_por_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_POR
#define _tk_del_por_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_CAL_POR
#define _tk_cal_por_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_ACP_POR
#define _tk_acp_por_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_FWD_POR
#define _tk_fwd_por_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_RPL_RDV
#define _tk_rpl_rdv_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_POR
#define _tk_ref_por_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_POR
#define _td_lst_por_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_POR
#define _td_ref_por_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_ACP_QUE
#define _td_acp_que_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_CAL_QUE
#define _td_cal_que_impl	_knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_MTX
#define _tk_cre_mtx_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_MTX
#define _tk_del_mtx_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_LOC_MTX
#define _tk_loc_mtx_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_UNL_MTX
#define _tk_unl_mtx_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_MTX
#define _tk_ref_mtx_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_MTX
#define _td_lst_mtx_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_MTX
#define _td_ref_mtx_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_MTX_QUE
#define _td_mtx_que_impl	_knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_MPL
#define _tk_cre_mpl_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_MPL
#define _tk_del_mpl_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_GET_MPL
#define _tk_get_mpl_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_REL_MPL
#define _tk_rel_mpl_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_MPL
#define _tk_ref_mpl_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_MPL
#define _td_lst_mpl_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_MPL
#define _td_ref_mpl_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_MPL_QUE
#define _td_mpl_que_impl	_knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_MPF
#define _tk_cre_mpf_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_MPF
#define _tk_del_mpf_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_GET_MPF
#define _tk_get_mpf_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_REL_MPF
#define _tk_rel_mpf_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_MPF
#define _tk_ref_mpf_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_MPF
#define _td_lst_mpf_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_MPF
#define _td_ref_mpf_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_MPF_QUE
#define _td_mpf_que_impl	_knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_CYC
#define _tk_cre_cyc_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_CYC
#define _tk_del_cyc_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_STA_CYC
#define _tk_sta_cyc_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_STP_CYC
#define _tk_stp_cyc_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_CYC
#define _tk_ref_cyc_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_CYC
#define _td_lst_cyc_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_CYC
#define _td_ref_cyc_impl	_knl_no_support
#endif

#ifndef USE_FUNC_TK_CRE_ALM
#define _tk_cre_alm_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_DEL_ALM
#define _tk_del_alm_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_STA_ALM
#define _tk_sta_alm_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_STP_ALM
#define _tk_stp_alm_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_ALM
#define _tk_ref_alm_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_ALM
#define _td_lst_alm_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_ALM
#define _td_ref_alm_impl	_knl_no_support
#endif

#ifndef USE_FUNC_TK_DEF_SSY
#define _tk_def_ssy_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_SSY
#define _tk_ref_ssy_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_LST_SSY
#define _td_lst_ssy_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_REF_SSY
#define _td_ref_ssy_impl	_knl_no_support
#endif

#ifndef USE_FUNC_TK_OPN_DEV
#define _tk_opn_dev_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_CLS_DEV
#define _tk_cls_dev_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_REA_DEV
#define _tk_rea_dev_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_SREA_DEV
#define _tk_srea_dev_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_WRI_DEV
#define _tk_wri_dev_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_SWRI_DEV
#define _tk_swri_dev_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_WAI_DEV
#define _tk_wai_dev_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_SUS_DEV
#define _tk_sus_dev_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_GET_DEV
#define _tk_get_dev_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_DEV
#define _tk_ref_dev_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_OREF_DEV
#define _tk_oref_dev_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_LST_DEV
#define _tk_lst_dev_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_EVT_DEV
#define _tk_evt_dev_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_DEF_DEV
#define _tk_def_dev_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TK_REF_IDV
#define _tk_ref_idv_impl	_knl_no_support
#endif

#ifndef USE_FUNC_TD_HOK_SVC
#define _td_hok_svc_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_HOK_DSP
#define _td_hok_dsp_impl	_knl_no_support
#endif
#ifndef USE_FUNC_TD_HOK_INT
#define _td_hok_int_impl	_knl_no_support
#endif

#endif /* _Csym == 0 */

#endif /* _ISYSCONF_ */

