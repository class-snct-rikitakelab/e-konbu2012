
#include "Notice.h"
#include "../Common/Factory.h"

void Notice_init(Notice *this_Notice){
	Notice_setCngSectTerm(this_Notice,mSector.cngTerm);
}

void Notice_copyTerm(Notice *this_Notice,CngSectTerm *term){
	int i=0;

	for(i=0;i<CNG_TERM_MAX_NUM;++i){
		this_Notice->term[i].jBT = term[i].jBT;
		this_Notice->term[i].targDist =term[i].targDist;
		this_Notice->term[i].jlineEdge = term[i].jlineEdge;
		this_Notice->term[i].targBatteryVol = term[i].targBatteryVol;
		this_Notice->term[i].targetGradient = term[i].targetGradient;
		this_Notice->term[i].targTime = term[i].targTime;
		this_Notice->term[i].targXCoo = term[i].targXCoo;
		this_Notice->term[i].targYCoo = term[i].targYCoo;
		this_Notice->term[i].makerThreshold = term[i].makerThreshold;
		this_Notice->term[i].targObstDist = term[i].targObstDist;
		this_Notice->term[i].targTurnAngle = term[i].targTurnAngle;
	}
}
	/**
	 * 区間切替条件指定 making flags
	 */
void Notice_setCngSectTerm(Notice *this_Notice,CngSectTerm *term){
	int i = 0;	

	Notice_copyTerm(this_Notice,term);

	//this_Notice->term = term; //最新の切替条件にNoticeクラスが持つ属性も更新

	//initiallize noice terms 
	for(i=0;i<CNG_TERM_MAX_NUM;++i){
		this_Notice->noticeTerm[i].fBT = false;
		this_Notice->noticeTerm[i].fDist =false;
		this_Notice->noticeTerm[i].fLineEdge = false;
		this_Notice->noticeTerm[i].fBattery = false;
		this_Notice->noticeTerm[i].fGradient = false;
		this_Notice->noticeTerm[i].fTime = false;
		this_Notice->noticeTerm[i].fXCoo = false;
		this_Notice->noticeTerm[i].fYCoo = false;
		this_Notice->noticeTerm[i].fMarker = false;
		this_Notice->noticeTerm[i].fDetcObst = false;
		this_Notice->noticeTerm[i].fTurnAngle = false;
	}

	for(i=0;i<CNG_TERM_MAX_NUM;++i){
		if(term[i].jBT == true){
			this_Notice->noticeTerm[i].fBT = true;
		}
		if(term[i].targDist !=0 ){
			this_Notice->noticeTerm[i].fDist =true;
		}
		
		if(term[i].jlineEdge ==true ){
			this_Notice->noticeTerm[i].fLineEdge =true;
		}

		if(term[i].targBatteryVol !=0 ){
			this_Notice->noticeTerm[i].fBattery =true;
		}
		if(term[i].targetGradient !=0 ){
			this_Notice->noticeTerm[i].fGradient =true;
		}
		if(term[i].targTime !=0 ){
			this_Notice->noticeTerm[i].fTime =true;
		}
		if(term[i].targXCoo !=0 ){
			this_Notice->noticeTerm[i].fXCoo =true;
		}
		if(term[i].targYCoo !=0 ){
			this_Notice->noticeTerm[i].fYCoo =true;
		}
		if(term[i].makerThreshold !=0 ){
			this_Notice->noticeTerm[i].fMarker =true;
		}
		if(term[i].targObstDist !=0 ){
			this_Notice->noticeTerm[i].fDetcObst =true;
		}
		if(term[i].targTurnAngle !=0 ){
			this_Notice->noticeTerm[i].fTurnAngle =true;
		}

	}
}

	/*
	 * 通知判断
	 */
void Notice_judgeNotice(Notice *this_Notice){
	int i=0;
	
	int numOfOnFlag[CNG_TERM_MAX_NUM]; //need to intialized?? 
 	int satisTermNum[CNG_TERM_MAX_NUM]; //need to intialized?? 
	
	//count number of flag
	for(i=0;i<CNG_TERM_MAX_NUM;++i){
		numOfOnFlag[i] = Notice_countOnFlag(this_Notice,this_Notice->noticeTerm[i]);
	}
	//区間切替条件のトリガーを満たした数をカウント	
	for(i=0;i<CNG_TERM_MAX_NUM;++i){
		Notice_setTargVal(this_Notice,this_Notice->term[i]); //検出条件を切り替える
		satisTermNum[i]= Notice_judgeStatisTerm(this_Notice,this_Notice->term[i],this_Notice->noticeTerm[i]);
	}
	

	//区間切替条件のトリガーを満たした数と、フラグ（使用検出器数）が一致した条件の名前で区間を切替
	for(i=0;i<CNG_TERM_MAX_NUM;++i){
		if(numOfOnFlag[i] == satisTermNum[i] &&numOfOnFlag[i]!=0 ) {
			Sound_soundTone(&mSound,250,100,20);
			RunDist_resetRunDist(&mRunDist); //距離計をリセット
			Timer_reset(&mTimer); //区間走行経過時間をリセット
			Coordinates_rest(&mCoordinates);//現在座標をバッファに保存　区間開始点を基準とするため
			//Running_cngNextSect(&mRunning,this_Notice->term[i].sectName);
		}
	}
	

}



int Notice_countOnFlag(Notice *this_Notice,NoticeTerm noticeTerm){
		int flagCounter=0;

		if(noticeTerm.fBT==true){
			flagCounter++;
		}
		if(noticeTerm.fLineEdge == true){
			flagCounter++;
		}
		if(noticeTerm.fDist ==true){
			flagCounter++;
		}
		if(noticeTerm.fBattery ==true){
			flagCounter++;
		}

		if(noticeTerm.fGradient == true){
			flagCounter++;
		}

		if(noticeTerm.fTime == true){
			flagCounter++;
		}
		if(noticeTerm.fXCoo == true && noticeTerm.fYCoo == true){
			flagCounter++;
		}
		/*
		if(noticeTerm.fYCoo == true){
			flagCounter++;

		}
		*/

		if(noticeTerm.fMarker == true){
			flagCounter++;

		}
		if(noticeTerm.fDetcObst == true){
			flagCounter++;
		}
		if(noticeTerm.fTurnAngle == true){
			flagCounter++;
		}
	return flagCounter;
}

int Notice_judgeStatisTerm(Notice *this_Notice,CngSectTerm term,NoticeTerm noticeTerm){


int satisTermNum=0;

		if(RunStartSigRecv_detcRunStartSig(&mRunStartSigRecv)==true&&noticeTerm.fBT==true){
			satisTermNum++;
		}
		if(RunDist_dectTargRunDist(&mRunDist)==true&noticeTerm.fDist==true) {
			satisTermNum++;
		}
		if( Gradient_dectTargGradient(&mGradient)==true&&noticeTerm.fDist==true) {
			satisTermNum++;
		}
		if(Timer_detcPassTargTimer(&mTimer)==true&&noticeTerm.fTime==true){
			satisTermNum++;
		}
		if(Cooridnates_detcXCooReach(&mCoordinates)==true&&noticeTerm.fXCoo==true&&Cooridnates_detcYCooReach(&mCoordinates)==true&&noticeTerm.fYCoo==true){
			satisTermNum++;
		}
		/*
		if(Cooridnates_detcYCooReach(&mCoordinates)==true&&noticeTerm.fYCoo==true){
			satisTermNum++;
		}
		*/
		if(DetcMarker_detcMarker(&mDetcMarker)==true&&noticeTerm.fMarker==true){
			satisTermNum++;
		}

		if(DetcObstacle_DetcObst(&mDetcObst)==true&&noticeTerm.fDetcObst==true){
			satisTermNum++;
		}
		if(TurnAngle_detcTurnAngle(&mTurnAngle)==true&&noticeTerm.fTurnAngle==true){
			satisTermNum++;
		}
		if(Battery_dectRedct(&mBattery)==true&&noticeTerm.fBattery==true){
			satisTermNum++;
		}
		if(LineEdge_detectLineEdge(&mLineEdge)==true){
			satisTermNum++;
		}	
		return satisTermNum;
}

void Notice_setTargVal(Notice *this_Notice,CngSectTerm term){
	
	mBattery.targReductRang=term.targBatteryVol;
	mCoordinates.targXCoo = term.targXCoo;
	mCoordinates.targYCoo = term.targYCoo;
	mDetcMarker.markerThreshold = term.makerThreshold;
	mDetcObst.dectDist=term.targObstDist;
	mGradient.targRange=term.targetGradient;
	mRunDist.targRunDist=term.targDist;
	mTimer.targTimer=term.targTime;
	mTurnAngle.targAngle=term.targTurnAngle;
}
