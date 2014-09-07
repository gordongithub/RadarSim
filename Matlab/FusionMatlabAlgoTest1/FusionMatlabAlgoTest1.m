function [ fusionDatas, filterDatas, controlDatas, globalVarsOutput ] = FusionMatlabAlgoTest1( planeTrueDatas, targetNoiseDatas, globalVars, interval )

% msg123 = msgbox(num2str(globalVars),'globalin');
% set(msg123,'Position',[1000 100 400 300]);


T = interval;
sigma = 0.005;                        %��������������
Q = sigma.^2;                   %������������
Onum = size(planeTrueDatas,1);                      %�һ�����
Tnum = size(targetNoiseDatas,1)/Onum;                          %�л�����

globalVarsMatrix = zeros(Tnum+Onum,25,25);
for k = 1:Tnum+Onum
    for i = 1:25
        globalVarsMatrix(k,:,i) = permute(globalVars(k,(25*i-24):25*i),[3 2 1]);         
    end
end
% globalVarsMatrixOutput = globalVarsMatrix;
globalVarsMatrixOutput = zeros(Tnum+Onum,25,25);

targetNoiseDatas(:,4:5) = targetNoiseDatas(:,4:5)/57.2957795130823208768;

for k = 1:Onum
    planePitch = atan(planeTrueDatas(k,7)/planeTrueDatas(k,6));
    planeYaw = atan(planeTrueDatas(k,8)/sqrt(planeTrueDatas(k,7)^2+planeTrueDatas(k,6)^2));
    targetNoiseDatas((4*k-3):4*k,4) = targetNoiseDatas((4*k-3):4*k,4) + permute([planePitch;planePitch;planePitch;planePitch],[1 3 2]);
    targetNoiseDatas((4*k-3):4*k,5) = targetNoiseDatas((4*k-3):4*k,5) + permute([planeYaw;planeYaw;planeYaw;planeYaw],[1 3 2]);
end

targetNoiseDatasTrans = zeros(Onum,Tnum,8);
for k = 1:Onum
    for i = 1:Tnum
        targetNoiseDatasTrans(k,i,:) = permute(targetNoiseDatas(4*k-4+i,:),[1 3 2]); 
    end    
end

REsm = [targetNoiseDatasTrans(1,1,7),targetNoiseDatasTrans(2,1,7),targetNoiseDatasTrans(3,1,7)];
RInf = [targetNoiseDatasTrans(1,1,7),targetNoiseDatasTrans(1,1,8),targetNoiseDatasTrans(2,1,7),targetNoiseDatasTrans(2,1,8),targetNoiseDatasTrans(3,1,7),targetNoiseDatasTrans(3,1,8)];
VarREsm = REsm' * REsm;
VarRInf = RInf' * RInf;
Association = zeros(Tnum,1);%��ʼ��Ŀ���������


if(planeTrueDatas(1,1)==0)
        globalVarsMatrix(1,1,1:11) = permute([0;100;400000;110;23000;-440;10;0;0;0;0],[3 2 1]);
        globalVarsMatrix(2,1,1:11) = permute([0;101;410000;24000;19000;-460;5;0;0;0;0],[3 2 1]);
        globalVarsMatrix(3,1,1:11) = permute([0;102;420000;40000;18000;-480;-5;0;0;0;0],[3 2 1]);
        globalVarsMatrix(4,1,1:11) = permute([0;103;430000;30300;17000;-480;0;0;0;0;0],[3 2 1]);
end
% msg10 = msgbox(num2str([globalVarsMatrix(1,1,1:11);globalVarsMatrix(2,1,1:11);globalVarsMatrix(3,1,1:11);globalVarsMatrix(4,1,1:11)]),'globalstate');
% set(msg10,'Position',[1000 100 300 150]);
%msgbox(num2str(targetNoiseDatasTrans(1:1, 2:2)));
%msgbox(num2str(targetNoiseDatasTrans(2:2, 2:2)));
%msgbox(num2str(targetNoiseDatasTrans(3:3, 2:2)));

%�����һ���л������룬����Ƿ��뿪Esm�˲�����
for k = 1:Tnum
    for i = 1:Onum
       distanceEsm(k,i) = ((globalVarsMatrix(k,1,3)-planeTrueDatas(i,3))^2+(globalVarsMatrix(k,1,4)-planeTrueDatas(i,4))^2+(globalVarsMatrix(k,1,5)-planeTrueDatas(i,5))^2)^0.5;
    end
end
DistanceMin = min(min(distanceEsm));
% msgbox('Message 0');
%Esm̽�ⷶΧ���˲�
if(DistanceMin>3e4)
 if(planeTrueDatas(1,1)~=0)
    %����ֵ
   for k=1:Tnum
      Target(k).ZEsm = zeros(Onum,1);
      Target(k).P_UKFEsm = permute(globalVarsMatrix(k,1:9,12:20),[3,2,1]);
   end
   
   %����ID�Ŷ�Esm���˲����й��� ��ȷ
   for k=1:Onum
       for i=1:Tnum
            TargetID = targetNoiseDatasTrans(k,i,2)+1-100;
            Target(TargetID).ZEsm(k,1) = targetNoiseDatasTrans(k,i,4); 
       end
   end
   %�����һ���λ�� ��ȷ
   for k=1:Onum
       Ownship(k).P = (planeTrueDatas(k,3:11))';
       Ownship(k).P = [Ownship(k).P(1,1);Ownship(k).P(4,1);Ownship(k).P(7,1);Ownship(k).P(2,1);Ownship(k).P(5,1);Ownship(k).P(8,1);Ownship(k).P(3,1);Ownship(k).P(6,1);Ownship(k).P(9,1)];
   end

   %����л���λ�� ��ȷ
   for k=1:Tnum
       Target(k).X_e_UKFEsm = permute(globalVarsMatrix(k,1,3:11),[3,2,1]);
       Target(k).X_e_UKFEsm = [Target(k).X_e_UKFEsm(1,1);Target(k).X_e_UKFEsm(4,1);Target(k).X_e_UKFEsm(7,1);Target(k).X_e_UKFEsm(2,1);Target(k).X_e_UKFEsm(5,1);Target(k).X_e_UKFEsm(8,1);Target(k).X_e_UKFEsm(3,1);Target(k).X_e_UKFEsm(6,1);Target(k).X_e_UKFEsm(9,1)];
   end
%     msg11 = msgbox(num2str([Ownship(1).P,Ownship(2).P,Ownship(3).P]),'Ownship');
%     set(msg11,'Position',[1000 500 300 150]);
%     msgbox(num2str([Target(1).ZEsm,Target(2).ZEsm,Target(3).ZEsm,Target(4).ZEsm]),'ZEsm');
%     msg123 = msgbox(num2str([Target(1).X_e_UKFEsm,Target(2).X_e_UKFEsm,Target(3).X_e_UKFEsm,Target(4).X_e_UKFEsm]),'xukfesm');
%     set(msg123,'Position',[500 100 300 150]);
%     msg1 = msgbox(num2str(Target(3).P_UKFEsm),'PUKFEsmafter');
%     set(msg1,'Position',[100 300 400 400]);
   %ESM-UKF�˲�
    for k = 1:Tnum
       [Target(k).X_e_UKFEsmNew,Target(k).P_UKFEsmNew]=UKF_Origin(T,Q,VarREsm,Target(k).X_e_UKFEsm,Target(k).ZEsm,Ownship,Target(k).P_UKFEsm);
       Target(k).X_e_UKFEsm = [Target(k).X_e_UKFEsmNew(1,1);Target(k).X_e_UKFEsmNew(4,1);Target(k).X_e_UKFEsmNew(7,1);Target(k).X_e_UKFEsmNew(2,1);Target(k).X_e_UKFEsmNew(5,1);Target(k).X_e_UKFEsmNew(8,1);Target(k).X_e_UKFEsmNew(3,1);Target(k).X_e_UKFEsmNew(6,1);Target(k).X_e_UKFEsmNew(9,1)];
       globalVarsMatrixOutput(k,1,1) = planeTrueDatas(1,1);
       globalVarsMatrixOutput(k,1,2) = 100+k-1;
       globalVarsMatrixOutput(k,1,3:11) = permute(Target(k).X_e_UKFEsm,[3,2,1]);
       Target(k).P_UKFEsm = Target(k).P_UKFEsmNew;
       globalVarsMatrixOutput(k,1:9,12:20) = permute(Target(k).P_UKFEsm,[3,2,1]);
    end
%     h = msgbox(num2str([Target(1).X_e_UKFEsm,Target(2).X_e_UKFEsm,Target(3).X_e_UKFEsm,Target(4).X_e_UKFEsm]),'XEUKFafter');
%     set(h,'Position',[100 100 300 300]);
       globalVarsOutput = zeros(Tnum+Onum,625);
       for k = 1:Tnum+Onum
           for i = 1:25
               globalVarsOutput(k,(25*i-24):25*i) = permute(globalVarsMatrixOutput(k,:,i),[3 2 1]);         
           end
       end
    %����ں�ֵ
    fusionDatasOutput = zeros(Tnum,11);
    filterDatasOutput = zeros(Tnum,11);
    controlDatas = zeros(Onum,3);
    for k = 1:Tnum
    fusionDatasOutput(k,1) = planeTrueDatas(1,1);
    fusionDatasOutput(k,2) = 100+k-1;
    fusionDatasOutput(k,3:11) = Target(k).X_e_UKFEsm';
    filterDatasOutput(k,1) = planeTrueDatas(1,1);
    filterDatasOutput(k,2) = 100+k-1;
    filterDatasOutput(k,3:11) = Target(k).X_e_UKFEsm';
    end
    fusionDatas = fusionDatasOutput;
    filterDatas = filterDatasOutput;
%     msg2 = msgbox(num2str(globalVarsOutput),'globalOut');
%     set(msg2,'Position',[1000 1000 400 300]);
%     msgbox(num2str(size(fusionDatas)),'fusion');
%     msgbox(num2str(size(filterDatas)),'filter');
%     msgbox('Message 1');
 else
   for k=1:Tnum
       Target(k).X_e_UKFEsm = permute(globalVarsMatrix(k,1,3:11),[3,2,1]);
       Target(k).P_UKFEsm = diag([1,0.01,0.0001,1,0.01,0.0001,1,0.01,0.0001]);
       globalVarsMatrixOutput(k,1,1) = planeTrueDatas(1,1);
       globalVarsMatrixOutput(k,1,2) = 100+k-1;
       globalVarsMatrixOutput(k,1,3:11) = permute(Target(k).X_e_UKFEsm,[3,2,1]);
       globalVarsMatrixOutput(k,1:9,12:20) = permute(Target(k).P_UKFEsm,[3,2,1]);
    end
       globalVarsOutput = zeros(Tnum+Onum,625);
       for k = 1:Tnum+Onum
           for i = 1:25
               globalVarsOutput(k,(25*i-24):25*i) = permute(globalVarsMatrixOutput(k,:,i),[3 2 1]);         
           end
       end
    fusionDatasOutput = zeros(Tnum,11);
    filterDatasOutput = zeros(Tnum,11);
    controlDatas = zeros(Onum,3);
    for k = 1:Tnum
       fusionDatasOutput(k,:) = (permute(globalVarsMatrix(k,1,1:11),[3 2 1]))';
       filterDatasOutput(k,:) = (permute(globalVarsMatrix(k,1,1:11),[3 2 1]))';
    end
    fusionDatas = fusionDatasOutput;
    filterDatas = filterDatasOutput;
 end
elseif(DistanceMin<3e4 && DistanceMin>2e4)
   %�����һ���λ��
   for k=1:Onum
       Ownship(k).P = (planeTrueDatas(k,3:11))';
       Ownship(k).P = [Ownship(k).P(1,1);Ownship(k).P(4,1);Ownship(k).P(7,1);Ownship(k).P(2,1);Ownship(k).P(5,1);Ownship(k).P(8,1);Ownship(k).P(3,1);Ownship(k).P(6,1);Ownship(k).P(9,1)];
   end
    %����ֵ
   for k=1:Tnum
      Target(k).ZInf = zeros(Onum*2,1);
      if(globalVarsMatrix(1,10,1:11)==permute(zeros(11,1),[3 2 1]))
            Target(k).P_UKFInf = diag([1,0.01,0.0001,1,0.01,0.0001,1,0.01,0.0001]);
            Target(k).X_e_UKFInf = permute(globalVarsMatrix(k,1,3:11),[3 2 1]);
            Target(k).X_e_UKFInf = [Target(k).X_e_UKFInf(1,1);Target(k).X_e_UKFInf(4,1);Target(k).X_e_UKFInf(7,1);Target(k).X_e_UKFInf(2,1);Target(k).X_e_UKFInf(5,1);Target(k).X_e_UKFInf(8,1);Target(k).X_e_UKFInf(3,1);Target(k).X_e_UKFInf(6,1);Target(k).X_e_UKFInf(9,1)];
      else
            Target(k).P_UKFInf = permute(globalVarsMatrix(k,10:18,12:20),[3 2 1]);
            Target(k).X_e_UKFInf = permute(globalVarsMatrix(k,10,3:11),[3 2 1]);
            Target(k).X_e_UKFInf = [Target(k).X_e_UKFInf(1,1);Target(k).X_e_UKFInf(4,1);Target(k).X_e_UKFInf(7,1);Target(k).X_e_UKFInf(2,1);Target(k).X_e_UKFInf(5,1);Target(k).X_e_UKFInf(8,1);Target(k).X_e_UKFInf(3,1);Target(k).X_e_UKFInf(6,1);Target(k).X_e_UKFInf(9,1)];
      end      
   end
   targetNoiseDatasTransTrans = permute(targetNoiseDatasTrans,[3 2 1]);%��������ά�����ת����Ϊ�˷��㴦������
   for i = 1:Onum
       for k = 1:Tnum
           Target(k).ZInf(2*i-1:2*i,1) = targetNoiseDatasTransTrans(4:5,k,i); 
       end
   end
   TargetDisorder(:,:,1) = [Target(1).ZInf(:,1),Target(2).ZInf(:,1),Target(3).ZInf(:,1),Target(4).ZInf(:,1)];
   AssociationCount = 0;
   %InfĿ�����
     for a = 1:Tnum
       for b = 1:Tnum
           for c = 1:Tnum
            TargetTheta = [TargetDisorder(1,a,1);TargetDisorder(3,b,1);TargetDisorder(5,c,1)];
            TargetPhi = [TargetDisorder(2,a,1);TargetDisorder(4,b,1);TargetDisorder(6,c,1)];
            OwnShipX = [Ownship(1).P(1,end);Ownship(2).P(1,end);Ownship(3).P(1,end)];
            OwnShipY = [Ownship(1).P(4,end);Ownship(2).P(4,end);Ownship(3).P(4,end)];
            OwnShipZ = [Ownship(1).P(7,end);Ownship(2).P(7,end);Ownship(3).P(7,end)];
            [X0,Y0,Z0,r] = Target_Association(OwnShipX,OwnShipY,OwnShipZ,TargetPhi,TargetTheta);
            FalsePoint = [X0;Y0;Z0];
            globalVarsMatrixOutput(a,19:21,(b-1)*Tnum+c) = permute(FalsePoint',[3 2 1]);
            if(r<3e3)
                Centre = [X0;Y0;Z0;r];
                Association = cat(2,Association,Centre);
                if(Association(:,1)==zeros(4,1))
                Association = Association(:,2:end); 
                end
                AssociationCount = AssociationCount+1;
            end
           end
       end
     end
     globalVarsMatrixOutput(1,22:24,1:AssociationCount) = permute(Association(1:3,:)',[3 2 1]);
   %Ŀ������
   AssociationInOrder = zeros(Tnum,1);
       for k = 1:Tnum
           for l = 1:size(Association,2)
               distance(l,1) = ((Target(k).X_e_UKFInf(1,end)-Association(1,l))^2 + (Target(k).X_e_UKFInf(4,end)-Association(2,l))^2)^0.5;           
           end
           [distancemin,distanceminID] = min(distance);
           AssociationInOrder(:,k) = Association(:,distanceminID);
       end
      
   %�Թ�����Ŀ����Ϊ����ֵ����Բ���ֵ���ʼ״̬
   Comparemin = 1;
   MapVector = zeros(Tnum,1);
   for k = 1:Tnum
       Target(k).ZInfComp = atan((Target(k).X_e_UKFInf(4,1)-Ownship(1).P(4,end))/(Target(k).X_e_UKFInf(1,1)-Ownship(1).P(1,end))); 
       ZInfEquivalent(:,1,k) = [atan((AssociationInOrder(2,k)-Ownship(1).P(4,end))/(AssociationInOrder(1,k)-Ownship(1).P(1,end)));atan((AssociationInOrder(3,k)-Ownship(1).P(7,end))/((AssociationInOrder(2,k)-Ownship(1).P(4,end))^2+(AssociationInOrder(1,k)-Ownship(1).P(1,end))^2)^0.5); ... ,
       atan((AssociationInOrder(2,k)-Ownship(2).P(4,end))/(AssociationInOrder(1,k)-Ownship(2).P(1,end)));atan((AssociationInOrder(3,k)-Ownship(2).P(7,end))/((AssociationInOrder(2,k)-Ownship(2).P(4,end))^2+(AssociationInOrder(1,k)-Ownship(2).P(1,end))^2)^0.5); ... ,
       atan((AssociationInOrder(2,k)-Ownship(3).P(4,end))/(AssociationInOrder(1,k)-Ownship(3).P(1,end)));atan((AssociationInOrder(3,k)-Ownship(3).P(7,end))/((AssociationInOrder(2,k)-Ownship(3).P(4,end))^2+(AssociationInOrder(1,k)-Ownship(3).P(1,end))^2)^0.5)];    
   end
   for k = 1:Tnum
       for l = 1:Tnum
           Compare = abs(Target(k).ZInfComp - ZInfEquivalent(1,1,l));
           if(Compare<Comparemin)
               Comparemin = Compare;
               MapVector(k,1) = l;
           end
       end
       Comparemin = 1;
       Target(k).ZInfEquivalent = ZInfEquivalent(:,:,MapVector(k,1));
   end
   %Inf-UKF�˲�
   for k = 1:Tnum
        [Target(k).X_e_UKFInfNew,Target(k).P_UKFInfNew]=UKF_Origin1(T,Q,VarRInf,Target(k).X_e_UKFInf,Target(k).ZInfEquivalent,Ownship,Target(k).P_UKFInf); %����ĳ�ʼ״̬�ȷ־��벿�ֵ�ͨ���޸�ΪEsm��ĩ״̬
        Target(k).X_e_UKFInf = [Target(k).X_e_UKFInfNew(1,1);Target(k).X_e_UKFInfNew(4,1);Target(k).X_e_UKFInfNew(7,1);Target(k).X_e_UKFInfNew(2,1);Target(k).X_e_UKFInfNew(5,1);Target(k).X_e_UKFInfNew(8,1);Target(k).X_e_UKFInfNew(3,1);Target(k).X_e_UKFInfNew(6,1);Target(k).X_e_UKFInfNew(9,1)];
        globalVarsMatrixOutput(k,10,1) = planeTrueDatas(1,1);
        globalVarsMatrixOutput(k,10,2) = 100+k-1;
        globalVarsMatrixOutput(k,10,3:11) = permute(Target(k).X_e_UKFInf,[3 2 1]);
        Target(k).P_UKFInf = Target(k).P_UKFInfNew;
        globalVarsMatrixOutput(k,10:18,12:20) = permute(Target(k).P_UKFInf,[3 2 1]);
   end
   globalVarsOutput = zeros(Tnum+Onum,625);
   for k = 1:Tnum+Onum
       for i = 1:25
           globalVarsOutput(k,(25*i-24):25*i) = permute(globalVarsMatrixOutput(k,:,i),[3 2 1]);         
       end
   end
    %����ں�ֵ
    fusionDatasOutput = zeros(Tnum,11);
    filterDatasOutput = zeros(Tnum,11);
    controlDatas = zeros(Onum,3);
    for k = 1:Tnum
       fusionDatasOutput(k,1) = planeTrueDatas(1,1);
       fusionDatasOutput(k,2) = 100+k-1;
       fusionDatasOutput(k,3:11) = Target(k).X_e_UKFInf';
       filterDatasOutput(k,1) = planeTrueDatas(1,1);
       filterDatasOutput(k,2) = 100+k-1;
       filterDatasOutput(k,3:11) = Target(k).X_e_UKFInf';
    end
    fusionDatas = fusionDatasOutput;
    filterDatas = filterDatasOutput;
    msgbox('Message 2');
end

