#include "DNN.h"

namespace MLL{

    /**
    ���в���������Ϊȫ�ֱ������ṹ�岻��Ҫ�ں���֮�䴫�ݣ�
    ���ݳ�������ʼ������
    **/
    int DNN::init_parameters()
    {
        int k=0,i=0,j=0;
        double radom = 0;
        int L=_sup_par.layer_dims;//�������
        parameters *p = &_par;//�������ṹ���Ѷ��岢�����ڴ棬�ṹ���ھ���δ�����ڴ�
        grad *g = &_gra;//�ݶȣ��ṹ���Ѷ��岢�����ڴ棬�ṹ���ھ���δ�����ڴ�
        /**
            �����ʼ��
        **/
        //p->A.initMatrix(sup_par.layer_n[k],X._col,0);

        //p->AT.initMatrix(X._col,X._row,0);
        for(k=0; k<L-1; k++)
        {
            p->A.initMatrix(_sup_par.layer_n[k],_x._col,0);
            //����dropout�������ʼ��һ�μ��ɣ����浱ʹ��dropoutʱ��D�Żḳֵ����ʹ���򲻸�ֵ����ʵ��ʹ�ó���С���������
            p->D.initMatrix(_sup_par.layer_n[k],_x._col,0);
            p->W.initMatrix(_sup_par.layer_n[k+1],_sup_par.layer_n[k],0);
            p->b.initMatrix(_sup_par.layer_n[k+1],1,0);
            p->Z.initMatrix(_sup_par.layer_n[k+1],_x._col,0);

            for(i=0; i<p->W._row; i++)
            {
                for(j=0; j<p->W._col; j++)
                {
                    if(_initialization=="he")
                    {
                        radom=(rand()%100)/100.0;
                        p->W._data[i][j]=radom * sqrt(2.0/_sup_par.layer_n[k]);//һ�ֳ��õĲ�����ʼ��������������ʼ��Ҳ�м���
                    }
                    if(_initialization=="random")
                    {
                        radom=(rand()%100)/100.0;
                        p->W._data[i][j]=radom;//һ�ֳ��õĲ�����ʼ��������������ʼ��Ҳ�м���
                    }
                    if(_initialization=="arxiv")
                    {
                        radom=(rand()%100)/100.0;
                        p->W._data[i][j]=radom/sqrt(_sup_par.layer_n[k]);//һ�ֳ��õĲ�����ʼ��������������ʼ��Ҳ�м���
                    }
                }
            }
            p->next=new parameters();//��һ���������
            p->next->pre=p;
            p=p->next;

            g->grad_A.initMatrix(_sup_par.layer_n[L-k-1],_x._col,0);
            g->grad_Z.initMatrix(_sup_par.layer_n[L-k-1],_x._col,0);
            g->grad_W.initMatrix(_sup_par.layer_n[L-k-1],_sup_par.layer_n[L-k-2],0);
            g->grad_b.initMatrix(_sup_par.layer_n[L-k-1],1,0);

            //����momentum ��adam�Ż������ڱ���ǰn�μ�Ȩƽ��ֵ
            g->V_dw.initMatrix(_sup_par.layer_n[L-k-1],_sup_par.layer_n[L-k-2],0);
            g->V_db.initMatrix(_sup_par.layer_n[L-k-1],1,0);

            g->S_dw.initMatrix(_sup_par.layer_n[L-k-1],_sup_par.layer_n[L-k-2],0);
            g->S_db.initMatrix(_sup_par.layer_n[L-k-1],1,0);

            //����������momentum ��adam
            g->V_dw_corrected.initMatrix(_sup_par.layer_n[L-k-1],_sup_par.layer_n[L-k-2],0);
            g->V_db_corrected.initMatrix(_sup_par.layer_n[L-k-1],1,0);

            g->S_dw_corrected.initMatrix(_sup_par.layer_n[L-k-1],_sup_par.layer_n[L-k-2],0);
            g->S_db_corrected.initMatrix(_sup_par.layer_n[L-k-1],1,0);

            g->pre=new grad();//��һ����������ݶ�
            g->pre->next=g;
            g=g->pre;
        }
        p->A.initMatrix(_sup_par.layer_n[k],_x._col,0);
        g->grad_A.initMatrix(_sup_par.layer_n[L-k-1],_x._col,0);

        return 0;
    }

    void DNN::line_forward(parameters *p,double keep_prob)
    {
        int i=0,j=0;
        if(keep_prob!=1)
        {
           for(i=0; i<p->D._row; i++)
            {
                for(j=0; j<p->D._col; j++)
                {
                    p->D._data[i][j]=(rand()%100)/100.0;

                    if(p->D._data[i][j]<keep_prob)
                        p->D._data[i][j]=1.0/keep_prob; //�����Ѿ�������keep_prob
                    else
                        p->D._data[i][j]=0;
                }
            }
            p->A = p->A * p->D;
        }
        std::cout<<"zzz"<<p->Z._row<<"&&"<<p->Z._col<<std::endl;
        std::cout<<"www"<<p->W._row<<"&&"<<p->W._col<<std::endl;
        std::cout<<"aaa"<<p->A._row<<"&&"<<p->A._col<<std::endl;
        std::cout<<"bbb"<<p->b._row<<"&&"<<p->b._col<<std::endl;
        p->Z.print();
        p->b.print();
        p->Z = p->W * p->A;

        for(i=0; i<p->Z._row; i++) //��������������ӣ�class��δд
        {
            for(j=0; j<p->Z._col; j++)
            {
                p->Z._data[i][j]+=p->b._data[i][0];//������԰�bҲ����Ϊ�ȴ�С�ľ���ÿ��һ��
            }
        }
    }

    void DNN::sigmoid_forward(parameters *p)
    {
        int i,j;
        for(i=0; i<p->Z._row; i++)
        {
            for(j=0; j<p->Z._col; j++)
            {
                p->next->A._data[i][j]=1.0/(1.0+exp(-p->Z._data[i][j]));//sigmoid(p->Z._data[i][j]);
            }
        }
    }

    void DNN::relu_forward(parameters *p)
    {
        int i,j;
        for(i=0; i<p->Z._row; i++)
        {
            for(j=0; j<p->Z._col; j++)
            {
                if(p->Z._data[i][j]>0)
                {
                    p->next->A._data[i][j] = p->Z._data[i][j];
                }
                else
                {
                    p->next->A._data[i][j]=0;
                }
            }
        }
    }

    void DNN::line_active_forward(parameters *p,std::string active, double keep_prob)
    {
        line_forward(p,keep_prob);
        if(active=="relu")
        {
            relu_forward(p);
        }
        if(active=="sigmoid")
        {
            sigmoid_forward(p);
        }
    }

    Matrix DNN::model_forward(double *keep_probs)
    {
        int i=0;
        int L=_sup_par.layer_dims;
        parameters *p = &_par;
        p->A = _x;
        for(i=0; i<L-1 && p->next!=NULL; i++)
        {
            line_active_forward(p,_sup_par.layer_active[i+1],keep_probs[i]);
            p=p->next;
        }
        return p->A;
    }
    void DNN::sigmoid_backword(parameters *p,grad *g)
    {
        int i=0,j=0;
        for(i=0; i<g->grad_A._row; i++)
        {
            for(j=0; j<g->grad_A._col; j++)
            {
                g->grad_Z._data[i][j]=g->grad_A._data[i][j]*p->A._data[i][j]*(1-p->A._data[i][j]);
            }
        }
    }

    void DNN::relu_backword(parameters *p,grad *g)
    {
        int i=0,j=0;
        for(i=0; i<g->grad_Z._row; i++)
        {
            for(j=0; j<g->grad_Z._col; j++)
            {
                if(p->pre->Z._data[i][j]>0)
                {
                    g->grad_Z._data[i][j]=g->grad_A._data[i][j];
                }
                else
                {
                    g->grad_Z._data[i][j]=0;
                }
            }
        }
    }

    void DNN::line_backword(parameters *p,grad *g, double keep_prob)
    {
        int i,j;
        Matrix AT = p->A.transposeMatrix();
        g->grad_W = g->grad_W.multsMatrix(g->grad_Z,AT);
        if(_lambd!=0)
        {
            for(i=0; i<p->W._row; i++)
            {
                for(j=0; j<p->W._col; j++)
                {
                     g->grad_W._data[i][j]+=(_lambd * p->W._data[i][j]);
                }
            }
        }
        for(i=0; i<g->grad_W._row; i++)
        {
            for(j=0; j<g->grad_W._col; j++)
            {
                g->grad_W._data[i][j]/=g->grad_Z._col;
            }
        }
        for(i=0; i<g->grad_Z._row; i++)
        {
            g->grad_b._data[i][0]=0;
            for(j=0; j<g->grad_Z._col; j++)
            {
                g->grad_b._data[i][0]+=g->grad_Z._data[i][j];
            }
            g->grad_b._data[i][0]/=g->grad_Z._col;
        }
        Matrix WT = p->W.transposeMatrix();
        g->pre->grad_A = g->pre->grad_A.multsMatrix(WT,g->grad_Z);
		if(keep_prob!=1)
        {
            //����pָ���D���ӦA��dropout�㣬������1������£�D��ֻ�г�ʼ�����޹ظ�ֵ�����Զ�Ӧdropout��ϵ����ȷ��
            //std::cout<<p->D._col<<"&"<<p->D._row<<std::endl;
            //std::cout<<g->pre->grad_A._col<<"&"<<g->pre->grad_A._row<<std::endl;

            g->pre->grad_A = g->pre->grad_A.multsMatrix(g->pre->grad_A,p->D);//����keep_prob�����Ѿ��ŵ�D����
        }
        //AT.clear();
        //WT.clear();
    }

    void DNN::line_active_backword(parameters *p,grad *g,std::string active, double keep_prob)
    {
		std::cout<<"active_backword_start"<<std::endl;
        if(active=="sigmoid")
        {
            sigmoid_backword(p,g);
        }
        if(active=="relu")
        {
            relu_backword(p,g);
        }
		std::cout<<"line_backword_start"<<std::endl;
        line_backword(p->pre,g,keep_prob);

    }
    void DNN::model_backword(Matrix AL,double *keep_probs)
    {
        int i=0,j=0;
        int L=_sup_par.layer_dims;

        parameters *p = &_par;
        while(p->next!=NULL)
        {
            p=p->next;
        }
        grad *g = &_gra;

        for(i=0; i< _y._col; i++)
        {
            _gra.grad_A._data[0][i]=-(_y._data[0][i]/AL._data[0][i]-(1 - _y._data[0][i])/(1-AL._data[0][i]));
        }
        for(i=L-1; i>0; i--)
        {
            line_active_backword(p,g,_sup_par.layer_active[i],keep_probs[i]);
            g=g->pre;
            p=p->pre;
        }
    }

    double DNN::cost_cumpter(Matrix AL)
    {
        int i=0,j=0;
        int m = _y._col;//������
        double loss=0;
        double loss_L2_regularization=0;
        if(_lambd!=0)
        {
            parameters *p = &_par;
            while(p!=NULL)
            {
                for(i=0;i<p->W._row;i++)
                {
                    for(j=0;j<p->W._col;j++)
                    {
                        loss_L2_regularization+=(_lambd*p->W._data[i][j]*p->W._data[i][j]);
                    }
                }
               p=p->next;
            }
            loss_L2_regularization/=(2*m);
        }
        for(i=0; i<m; i++)
        {
            loss+=-(_y._data[0][i]*log(AL._data[0][i])+(1 - _y._data[0][i])*log(1-AL._data[0][i]));
        }
        loss/=m;
        //loss+=loss_L2_regularization;
        return loss;
    }
    int DNN::updata_parameters_with_gd(int t)
    {
        int k=0,i=0,j=0;
        int L = _sup_par.layer_dims;
        parameters *p = &_par;
        grad *g = &_gra;
        while(g->pre->pre!=NULL)//�����ǰ����ϲ����ķ�ʽ��ͬ��ǰ��A=sgn(WX+b),���� AL=sgn(WA+b)
        {
            g=g->pre;
        }

        //learn_rateing *= pow(0.999, t/1000);//ָ��˥��

        for(k=0; k<L-1&&p->next!=NULL&&g!=NULL; k++)
        {
            for(i=0; i<g->grad_W._row; i++)
            {
                g->grad_b._data[i][0] *= -_learn_rateing;
                for(j=0; j<g->grad_W._col; j++)
                {
                    g->grad_W._data[i][j] *= -_learn_rateing;
                }
            }
            p->W = p->W + g->grad_W;
            p->b = p->b + g->grad_b;
            p=p->next;
            g=g->next;
        }
        return 0;
    }

    int DNN::updata_parameters_with_momentum(int t)
    {
        int k=0,i=0,j=0;
        int L = _sup_par.layer_dims;
        parameters *p = &_par;
        grad *g = &_gra;
        while(g->pre->pre!=NULL)//�����ǰ����ϲ����ķ�ʽ��ͬ��ǰ��A=sgn(WX+b),���� AL=sgn(WA+b)
        {
            g=g->pre;
        }

        //learn_rateing *= pow(0.999, t/1000);//ָ��˥��

        for(k=0; k<L-1&&p->next!=NULL&&g!=NULL; k++)
        {
            for(i=0; i<g->grad_W._row; i++)
            {
                g->V_db._data[i][0]=(_beta1 * g->V_db._data[i][0] + (1 - _beta1) * g->grad_b._data[i][0]);
                g->V_db_corrected._data[i][0] = g->V_db._data[i][0] / (1-pow(_beta1,t));//����
                g->grad_b._data[i][0]=(-_learn_rateing) * g->V_db_corrected._data[i][0];

                for(j=0; j<g->grad_W._col; j++)
                {
                    g->V_dw._data[i][j]=(_beta1 * g->V_dw._data[i][j] + (1 - _beta1) * g->grad_W._data[i][j]);
                    g->V_dw_corrected._data[i][j]=g->V_dw._data[i][j] / (1-pow(_beta1,t));//����
                    g->grad_W._data[i][j]=(-_learn_rateing) * g->V_dw_corrected._data[i][j];
                }
            }
            p->W = p->W + g->grad_W;
            p->b = p->b + g->grad_b;
            p=p->next;
            g=g->next;
        }
        return 0;
    }

    int DNN::updata_parameters_with_adam(int t)
    {
        int k=0,i=0,j=0;
        int L = _sup_par.layer_dims;
        parameters *p = &_par;
        grad *g= &_gra;
        while(g->pre->pre!=NULL)//�����ǰ����ϲ����ķ�ʽ��ͬ��ǰ��A=sgn(WX+b),���� AL=sgn(WA+b)
        {
            g=g->pre;
        }

        //learn_rateing *= pow(0.999, t/1000);//ָ��˥��

        for(k=0; k<L-1&&p->next!=NULL&&g!=NULL; k++)
        {
            for(i=0; i<g->grad_W._row; i++)
            {
                g->V_db._data[i][0]=(_beta1 * g->V_db._data[i][0] + (1 - _beta1) * g->grad_b._data[i][0]);
                g->V_db_corrected._data[i][0] = g->V_db._data[i][0] / (1-pow(_beta1,t));//����

                g->S_db._data[i][0]=(_beta2 * g->S_db._data[i][0] + (1 - _beta2) * (g->grad_b._data[i][0] * g->grad_b._data[i][0]));
                g->S_db_corrected._data[i][0] = g->S_db._data[i][0] / (1-pow(_beta2,t));//����

                g->grad_b._data[i][0]= (-_learn_rateing) * g->V_db_corrected._data[i][0] / sqrt(g->S_db_corrected._data[i][0]);

                for(j=0; j<g->grad_W._col; j++)
                {
                    g->V_dw._data[i][j]=(_beta1 * g->V_dw._data[i][j] + (1 - _beta1) * g->grad_W._data[i][j]);
                    g->V_dw_corrected._data[i][j]=g->V_dw._data[i][j] / (1-pow(_beta1,t));//����

                    g->S_dw._data[i][j]=(_beta2 * g->S_dw._data[i][j] + (1 - _beta2) * (g->grad_W._data[i][j] * g->grad_W._data[i][j]));
                    g->S_dw_corrected._data[i][j]=g->S_dw._data[i][j] / (1-pow(_beta2,t));//����

                    g->grad_W._data[i][j]= (-_learn_rateing) * g->V_dw_corrected._data[i][j] / sqrt(g->S_dw_corrected._data[i][j] + _epsilon) ;
                }
            }
            p->W = p->W + g->grad_W;
            p->b = p->b + g->grad_b;
            p=p->next;
            g=g->next;
        }
        return 0;
    }

    int DNN::updata_parameters(int t)
    {

        if(_optimizer=="gd")
           updata_parameters_with_gd(t);
        else if(_optimizer == "momentum")
            updata_parameters_with_momentum(t);
        else if(_optimizer =="adam")
            updata_parameters_with_adam(t);
        return 0;
    }
    int DNN::predict()
    {
        int i,k;
        int L = _sup_par.layer_dims;
        //parameters *p;
        //p = &_par;
        //p->A = _x.copyMatrix();
        //Matrix AL;
        Matrix AL(_y._row,_y._col,0);
        double *keep_probs=new double [L];
        for(k=0;k<L;k++)
        {
            keep_probs[k]=1;
        }
        AL=model_forward(keep_probs);
        for(i=0;i<_y._col;i++)
        {
            if(AL._data[0][i]>0.5)
                AL._data[0][i]=1;
            else
                AL._data[0][i]=0;
        }
        double pre=0;
        for(i=0;i<_y._col;i++)
        {
            if((AL._data[0][i]==1 && _y._data[0][i]==1)||(AL._data[0][i]==0 && _y._data[0][i]==0))
                pre+=1;
        }
        pre/=_y._col;
        std::cout<<"pre="<<pre<<std::endl;
        return 0;
    }

    DNN::DNN(const std::string &file, const char *optimizer,double learn_rateing,const char *initialization, double lambd, double keep_prob, \
            int mini_batch_size,double beta1, double beta2, double epsilon, int iter, bool print_cost)
    {
        /**
        ��ʼ������

        **/

        _x.init_by_data(file);
        _x = _x.transposeMatrix();
        _x.print();
        //_x = _x.transposeMatrix();
        
        _y = _x.getOneRow(_x._row-1);
        _y.print();
        _x.deleteOneRow(_x._row-1);
        //_y=one_hot(_y,2);
        //_y.print();
        std::cout<<"_x:row&col"<<_x._row << _x._col<<std::endl; 
        std::cout<<"_y:row&col"<<_y._row << _y._col<<std::endl; 
        
        _initialization = initialization;
        _learn_rateing = learn_rateing;
        _optimizer = optimizer;
        _beta1 = beta1;
        _beta2 = beta2;
        _epsilon = epsilon;
        _lambd = lambd;
        _iter = iter;
        _print_cost = print_cost;
        _keep_prob = keep_prob; 
         
        int i=0,k=0;
        int lay_dim=3;
        int lay_n[3]= {500,3,1};
        lay_n[0]=_x._row;
        std::string lay_active[3]= {"relu","relu","sigmoid"};
        _sup_par.layer_dims=lay_dim;
        for(i=0; i<lay_dim; i++)
        {
            _sup_par.layer_n[i]=lay_n[i];
            _sup_par.layer_active[i]=lay_active[i];
        }
        init_parameters();
        double loss;

        //Matrix AL(_y._row,_y._col,0);
        Matrix AL(1,_y._col,0);
        double *keep_probs;
        keep_probs=new double [_sup_par.layer_dims];
        if(keep_prob==1)
        {
            for(k=0;k < _sup_par.layer_dims;k++)
            {
                keep_probs[k]=1;
            }
        }
        else if (keep_prob<1)
        {
            for(k=0;k<_sup_par.layer_dims;k++)
            {
                if(k==0 || k==_sup_par.layer_dims-1)
                {
                    keep_probs[k]=1;
                }
                else
                {
                    keep_probs[k]=1;
                }
            }
        }
        for(i=0; i<iter; i++)
        {
            std::cout<<"-----------forward------------"<<"i="<<i<<std::endl;
            AL=model_forward(keep_probs);
            std::cout<<"-----------loss--------------"<<std::endl;
            loss=cost_cumpter(AL);
            if(i%100==0)
                std::cout<<"loss="<<loss<<std::endl;
            std::cout<<"-----------backword-----------"<<std::endl;
            model_backword(AL,keep_probs);

            std::cout<<"-----------update--------------"<<std::endl;
            updata_parameters(i+1);
        }
        AL=model_forward(keep_probs);
        std::cout<<"train_end"<<std::endl;
        predict();
    }

}
