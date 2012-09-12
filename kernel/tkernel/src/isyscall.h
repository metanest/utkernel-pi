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
 *	isyscall.h
 *	System Call Int. Format Definition
 */

#ifndef _ISYSCALL_
#define _ISYSCALL_

/* ------------------------------------------------------------------------ */

#if TA_GP
# define _CALL(p1, p2, p3, hdr, cb)	\
		CallUserHandler((INT)(p1), (INT)(p2), (INT)(p3), (FP)(hdr), (cb)->gp)
# define CallUserHandlerP1(   p1,         hdr, cb)	_CALL(p1, 0,  0,  hdr, cb)
# define CallUserHandlerP2(   p1, p2,     hdr, cb)	_CALL(p1, p2, 0,  hdr, cb)
# define CallUserHandlerP3(   p1, p2, p3, hdr, cb)	_CALL(p1, p2, p3, hdr, cb)
# define CallUserHandlerP2_GP(p1, p2,     hdr, cb)	_CALL(p1, p2, gp, hdr, cb)
#else
# define CallUserHandlerP1(   p1,         hdr, cb)	(*(hdr))(p1)
# define CallUserHandlerP2(   p1, p2,     hdr, cb)	(*(hdr))(p1, p2)
# define CallUserHandlerP3(   p1, p2, p3, hdr, cb)	(*(hdr))(p1, p2, p3)
# define CallUserHandlerP2_GP(p1, p2,     hdr, cb)	(*(hdr))(p1, p2)
#endif

/* ------------------------------------------------------------------------ */

#if TA_GP
#define P0(void)		( int _1,int _2,int _3,int _4,int _5, VP gp )
#define P1(p1)			( p1,    int _2,int _3,int _4,int _5, VP gp )
#define P2(p1, p2)		( p1, p2,       int _3,int _4,int _5, VP gp )
#define P3(p1, p2, p3)		( p1, p2, p3,          int _4,int _5, VP gp )
#define P4(p1, p2, p3, p4)	( p1, p2, p3, p4,             int _5, VP gp )
#define P5(p1, p2, p3, p4, p5)	( p1, p2, p3, p4, p5,                 VP gp )
#define P2GP(p1, p2)		( p1, p2,			      VP gp )
#else
#define P0(void)		( void )
#define P1(p1)			( p1 )
#define P2(p1, p2)		( p1, p2 )
#define P3(p1, p2, p3)		( p1, p2, p3 )
#define P4(p1, p2, p3, p4)	( p1, p2, p3, p4 )
#define P5(p1, p2, p3, p4, p5)	( p1, p2, p3, p4, p5 )
#define P2GP(p1, p2)		( p1, p2 )
#endif

/* ------------------------------------------------------------------------ */

/* micro T-Kernel */
IMPORT ID tk_cre_tsk_impl P1( T_CTSK *pk_ctsk );
IMPORT ER tk_del_tsk_impl( ID tskid );
IMPORT ER tk_sta_tsk_impl( ID tskid, INT stacd );
IMPORT void tk_ext_tsk_impl( void );
IMPORT void tk_exd_tsk_impl( void );
IMPORT ER tk_ter_tsk_impl( ID tskid );
IMPORT ER tk_dis_dsp_impl( void );
IMPORT ER tk_ena_dsp_impl( void );
IMPORT ER tk_chg_pri_impl( ID tskid, PRI tskpri );
IMPORT ER tk_rot_rdq_impl( PRI tskpri );
IMPORT ER tk_rel_wai_impl( ID tskid );
IMPORT ID tk_get_tid_impl( void );
IMPORT ER tk_get_reg_impl( ID tskid, T_REGS *pk_regs, T_EIT *pk_eit, T_CREGS *pk_cregs );
IMPORT ER tk_set_reg_impl( ID tskid, T_REGS *pk_regs, T_EIT *pk_eit, T_CREGS *pk_cregs );
IMPORT ER tk_ref_tsk_impl( ID tskid, T_RTSK *pk_rtsk );
IMPORT ER tk_sus_tsk_impl( ID tskid );
IMPORT ER tk_rsm_tsk_impl( ID tskid );
IMPORT ER tk_frsm_tsk_impl( ID tskid );
IMPORT ER tk_slp_tsk_impl( TMO tmout );
IMPORT ER tk_wup_tsk_impl( ID tskid );
IMPORT INT tk_can_wup_impl( ID tskid );

#if CFN_MAX_SEMID > 0
IMPORT ID tk_cre_sem_impl( T_CSEM *pk_csem );
IMPORT ER tk_del_sem_impl( ID semid );
IMPORT ER tk_sig_sem_impl( ID semid, INT cnt );
IMPORT ER tk_wai_sem_impl( ID semid, INT cnt, TMO tmout );
IMPORT ER tk_ref_sem_impl( ID semid, T_RSEM *pk_rsem );
#endif

#if CFN_MAX_MTXID > 0
IMPORT ID tk_cre_mtx_impl( T_CMTX *pk_cmtx );
IMPORT ER tk_del_mtx_impl( ID mtxid );
IMPORT ER tk_loc_mtx_impl( ID mtxid, TMO tmout );
IMPORT ER tk_unl_mtx_impl( ID mtxid );
IMPORT ER tk_ref_mtx_impl( ID mtxid, T_RMTX *pk_rmtx );
#endif

#if CFN_MAX_FLGID > 0
IMPORT ID tk_cre_flg_impl( T_CFLG *pk_cflg );
IMPORT ER tk_del_flg_impl( ID flgid );
IMPORT ER tk_set_flg_impl( ID flgid, UINT setptn );
IMPORT ER tk_clr_flg_impl( ID flgid, UINT clrptn );
IMPORT ER tk_wai_flg_impl( ID flgid, UINT waiptn, UINT wfmode, UINT *p_flgptn, TMO tmout );
IMPORT ER tk_ref_flg_impl( ID flgid, T_RFLG *pk_rflg );
#endif

#if CFN_MAX_MBXID > 0
IMPORT ID tk_cre_mbx_impl( T_CMBX* pk_cmbx );
IMPORT ER tk_del_mbx_impl( ID mbxid );
IMPORT ER tk_snd_mbx_impl( ID mbxid, T_MSG *pk_msg );
IMPORT ER tk_rcv_mbx_impl( ID mbxid, T_MSG **ppk_msg, TMO tmout );
IMPORT ER tk_ref_mbx_impl( ID mbxid, T_RMBX *pk_rmbx );
#endif

#if CFN_MAX_MBFID > 0
IMPORT ID tk_cre_mbf_impl( T_CMBF *pk_cmbf );
IMPORT ER tk_del_mbf_impl( ID mbfid );
IMPORT ER tk_snd_mbf_impl( ID mbfid, VP msg, INT msgsz, TMO tmout );
IMPORT INT tk_rcv_mbf_impl( ID mbfid, VP msg, TMO tmout );
IMPORT ER tk_ref_mbf_impl( ID mbfid, T_RMBF *pk_rmbf );
#endif

#if CFN_MAX_PORID > 0
IMPORT ID tk_cre_por_impl( T_CPOR *pk_cpor );
IMPORT ER tk_del_por_impl( ID porid );
IMPORT INT tk_cal_por_impl( ID porid, UINT calptn, VP msg, INT cmsgsz, TMO tmout );
IMPORT INT tk_acp_por_impl( ID porid, UINT acpptn, RNO *p_rdvno, VP msg, TMO tmout );
IMPORT ER tk_fwd_por_impl( ID porid, UINT calptn, RNO rdvno, VP msg, INT cmsgsz );
IMPORT ER tk_rpl_rdv_impl( RNO rdvno, VP msg, INT rmsgsz );
IMPORT ER tk_ref_por_impl( ID porid, T_RPOR *pk_rpor );
#endif

IMPORT ER tk_def_int_impl P2( UINT dintno, T_DINT *pk_dint );
IMPORT void tk_ret_int_impl( void );

#if CFN_MAX_MPLID > 0
IMPORT ID tk_cre_mpl_impl( T_CMPL *pk_cmpl );
IMPORT ER tk_del_mpl_impl( ID mplid );
IMPORT ER tk_get_mpl_impl( ID mplid, W blksz, VP *p_blk, TMO tmout );
IMPORT ER tk_rel_mpl_impl( ID mplid, VP blk );
IMPORT ER tk_ref_mpl_impl( ID mplid, T_RMPL *pk_rmpl );
#endif

#if CFN_MAX_MPFID > 0
IMPORT ID tk_cre_mpf_impl( T_CMPF *pk_cmpf );
IMPORT ER tk_del_mpf_impl( ID mpfid );
IMPORT ER tk_get_mpf_impl( ID mpfid, VP *p_blf, TMO tmout );
IMPORT ER tk_rel_mpf_impl( ID mpfid, VP blf );
IMPORT ER tk_ref_mpf_impl( ID mpfid, T_RMPF *pk_rmpf );
#endif

IMPORT ER tk_set_tim_impl( SYSTIM *pk_tim );
IMPORT ER tk_get_tim_impl( SYSTIM *pk_tim );
IMPORT ER tk_get_otm_impl( SYSTIM *pk_tim );
IMPORT ER tk_dly_tsk_impl( RELTIM dlytim );

#if CFN_MAX_CYCID > 0
IMPORT ID tk_cre_cyc_impl P1( T_CCYC *pk_ccyc );
IMPORT ER tk_del_cyc_impl( ID cycid );
IMPORT ER tk_sta_cyc_impl( ID cycid );
IMPORT ER tk_stp_cyc_impl( ID cycid );
IMPORT ER tk_ref_cyc_impl( ID cycid, T_RCYC *pk_rcyc );
#endif

#if CFN_MAX_ALMID > 0
IMPORT ID tk_cre_alm_impl P1( T_CALM *pk_calm );
IMPORT ER tk_del_alm_impl( ID almid );
IMPORT ER tk_sta_alm_impl( ID almid, RELTIM almtim );
IMPORT ER tk_stp_alm_impl( ID almid );
IMPORT ER tk_ref_alm_impl( ID almid, T_RALM *pk_ralm );
#endif

IMPORT ER tk_ref_ver_impl( T_RVER *pk_rver );
IMPORT ER tk_ref_sys_impl( T_RSYS *pk_rsys );

#if CFN_MAX_SSYID > 0
IMPORT ER tk_def_ssy_impl P2( ID ssid, T_DSSY *pk_dssy );
IMPORT ER tk_ref_ssy_impl( ID ssid, T_RSSY *pk_rssy );
#endif

#if CFN_MAX_REGDEV > 0
IMPORT ID tk_opn_dev_impl( UB *devnm, UINT omode );
IMPORT ER tk_cls_dev_impl( ID dd, UINT option );
IMPORT ID tk_rea_dev_impl( ID dd, W start, VP buf, W size, TMO tmout );
IMPORT ER tk_srea_dev_impl( ID dd, W start, VP buf, W size, W *asize );
IMPORT ID tk_wri_dev_impl( ID dd, W start, VP buf, W size, TMO tmout );
IMPORT ER tk_swri_dev_impl( ID dd, W start, VP buf, W size, W *asize );
IMPORT ID tk_wai_dev_impl( ID dd, ID reqid, W *asize, ER *ioer, TMO tmout );
IMPORT INT tk_sus_dev_impl( UINT mode );
IMPORT ID tk_get_dev_impl( ID devid, UB *devnm );
IMPORT ID tk_ref_dev_impl( UB *devnm, T_RDEV *rdev );
IMPORT ID tk_oref_de_implv( ID dd, T_RDEV *rdev );
IMPORT INT tk_lst_dev_impl( T_LDEV *ldev, INT start, INT ndev );
IMPORT INT tk_evt_dev_impl( ID devid, INT evttype, VP evtinf );
IMPORT ID tk_def_dev_impl P3( UB *devnm, T_DDEV *ddev, T_IDEV *idev );
IMPORT ER tk_ref_idv_impl(T_IDEV *idev );
#endif

#endif /* _ISYSCALL_ */
