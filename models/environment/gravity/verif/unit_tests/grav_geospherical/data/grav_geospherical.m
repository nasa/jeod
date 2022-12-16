function [Pot,accel,dgdx] = grav_geospherical(pfix,gv)

% compute potential and accel in geospherical coordinates
% and transform to ECF.
% wont work at or very near poles ("A" becomes zero) ???

% compute spherical coordinates
r = norm(pfix);
A = sqrt(pfix(1)^2+pfix(2)^2);
phi = asin(pfix(3)/r);
lam = acos(pfix(1)/A);
if pfix(2)<0
    lam = 2*pi-lam;
end

% compute legendre polynomials (normalized)
P = zeros(gv.degree+1,gv.degree+3);
x = sin(phi);
P(0+1,0+1)=1; P(0+1,1+1)=0;
P(1+1,0+1)=sqrt(3)*x; P(1+1,1+1)=sqrt(3)*sqrt(1-x^2); P(1+1,2+1)=0.0;
for n=2:gv.degree
    for m=0:n-1
        f1 = sqrt((2*n-1)*(2*n+1)/(n-m)/(n+m));
        f2 = sqrt((n+m-1)*(2*n+1)*(n-m-1)/(n+m)/(n-m)/(2*n-3));
        P(n+1,m+1) = x*f1*P(n,m+1)-f2*P(n-1,m+1);
    end
    P(n+1,n+1) = sqrt(1-x^2)*sqrt((2*n+1)/2/n)*P(n,n);
    P(n+1,n+2) = 0.0;
end

% temp trash
% nn = 40;
% mm = 1;  % mm>0
% Nnm   = sqrt(factorial(nn+mm)/2/(2*nn+1)/factorial(nn-mm));
% Nnmp1 = sqrt(factorial(nn+mm+1)/2/(2*nn+1)/factorial(nn-mm-1));
% Nnmp2 = sqrt(factorial(nn+mm+2)/2/(2*nn+1)/factorial(nn-mm-2));
% PA_unnorm = Nnmp2*P(nn+1,mm+2+1)
% PB_unnorm = 2*(mm+1)*tan(phi)*Nnmp1*P(nn+1,mm+1+1) - (nn+mm+1)*(nn-mm)*Nnm*P(nn+1,mm+1)



% zero the sums
sums = zeros(13);

for n=2:gv.degree
    
    if gv.order<n
        Lim = gv.order;
    else
        Lim = n;
    end
    
    R = (gv.Re/r)^n;
    
    for m=0:Lim
        if m==0
            k=1;
        else
            k=2;
        end
        
        C  = gv.Cnm(n+1,m+1)*cos(m*lam)+gv.Snm(n+1,m+1)*sin(m*lam);
        S  = gv.Snm(n+1,m+1)*cos(m*lam)-gv.Cnm(n+1,m+1)*sin(m*lam);
        F  = sqrt(k/2*(n+m+1)*(n-m));
        G  = F*sqrt((n+m+2)*(n-m-1));
        D1 = F*P(n+1,m+2)-m*tan(phi)*P(n+1,m+1);
        %D2 = G*P(n+1,m+3)-(m+1)*tan(phi)*F*P(n+1,m+2);
        D2 = (m+1)*tan(phi)*F*P(n+1,m+2)-(n+m+1)*(n-m)*P(n+1,m+1); % ???????????
        
        % potential
        sums(1) = sums(1) + R*C*P(n+1,m+1);
        
        % accel (r,lam,phi)
        sums(2) = sums(2) + R*(n+1)*C*P(n+1,m+1);
        sums(3) = sums(3) + R*m*S*P(n+1,m+1);
        sums(4) = sums(4) + R*C*D1;
        
        % gradient partials
        sums(5) = sums(5) + R*(n+2)*(n+1)*C*P(n+1,m+1);  % dardr ?
        sums(6) = sums(6) + R*(n+1)*m*S*P(n+1,m+1);      % dardlam ?
        sums(7) = sums(7) + R*(n+1)*C*D1;                % dardphi ?
        
        sums(8)  = sums(8)  + R*(n+2)*m*S*P(n+1,m+1);    % dalamdr ?
        sums(9)  = sums(9)  + R*(m^2)*C*P(n+1,m+1);      % dalamdlam ?
        sums(10) = sums(10) + R*m*S*D1;                  % dalamdphi ?
                
        sums(11) = sums(11) + R*(n+2)*C*D1;              % daphidr ?
        sums(12) = sums(12) + R*m*S*D1;                  % daphidlam ?
        sums(13) = sums(13) + R*C*(D2-m*tan(phi)*D1-m/cos(phi)^2*P(n+1,m+1));  % daphidphi ?
        
    end
end

sums(2) = -gv.mu/r^2*(gv.full_field + sums(2)); % r
sums(3) =  gv.mu/r^2/cos(phi)*sums(3);          % lambda
sums(4) =  gv.mu/r^2*sums(4);                   % phi

sums(5) =  gv.mu/r^3*(2*gv.full_field+sums(5));
sums(6) = -gv.mu/r^2*sums(6);
sums(7) = -gv.mu/r^2*sums(7);

sums(8)  = -gv.mu/r^3/cos(phi)*sums(8);
sums(9)  = -gv.mu/r^2/cos(phi)*sums(9);
sums(10) =  sums(3)*tan(phi) + gv.mu/r^2/cos(phi)*sums(10);

sums(11) = -gv.mu/r^3*sums(11);
sums(12) =  gv.mu/r^2*sums(12);
sums(13) =  gv.mu/r^2*sums(13);

acc_sph(1,1) =  sums(2);
acc_sph(2,1) =  sums(3);
acc_sph(3,1) =  sums(4);

sums(1) = gv.mu/r*(gv.full_field+sums(1));
Pot = sums(1);

% transform to ECF
T(1,1:3) = [cos(lam)*cos(phi) -sin(lam) -cos(lam)*sin(phi)]; 
T(2,1:3) = [sin(lam)*cos(phi) cos(lam) -sin(lam)*sin(phi)]; 
T(3,1:3) = [sin(phi) 0 cos(phi)];

accel = T*acc_sph;

% gravity gradient
dgdx = zeros(3,3);

dgdx(1,1) = sums(5);
dgdx(2,1) = sums(8);
dgdx(3,1) = sums(11);

dgdx(1,2) = 1/r/cos(phi)*sums(6)-sums(3)/r;
dgdx(2,2) = 1/r/cos(phi)*sums(9)+sums(2)/r-tan(phi)*sums(4)/r;
dgdx(3,2) = 1/r/cos(phi)*sums(12)+tan(phi)*sums(3)/r;

dgdx(1,3) = 1/r*(sums(7)-sums(4));
dgdx(2,3) = 1/r*sums(10);
dgdx(3,3) = 1/r*(sums(13)+sums(2));

% transform to ECF
dgdx = T*dgdx*T';
