#include <m_pd.h>
typedef struct peakctl
{
    t_sample c_x;
    t_sample c_coef;
} t_peakctl;

typedef struct peak_tilde
{
    t_object x_obj;
    t_float x_sr;
    t_float x_hz;
    t_peakctl x_cspace;
    t_peakctl *x_ctl;
    t_float x_f;
} t_peak_tilde;
t_class *peak_tilde_class;
static void peak_tilde_ft1(t_peak_tilde *x, t_floatarg f);
static void *peak_tilde_new(t_floatarg f)
{
    t_peak_tilde *x = (t_peak_tilde *)pd_new(peak_tilde_class);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("float"), gensym("ft1"));
    outlet_new(&x->x_obj, &s_signal);
    x->x_sr = 44100;
    x->x_ctl = &x->x_cspace;
    x->x_cspace.c_x = 0;
    peak_tilde_ft1(x, f);
    x->x_f = 0;
    return (x);
}
static void peak_tilde_ft1(t_peak_tilde *x, t_floatarg f)
{
    if (f < 0) f = 0;
    x->x_hz = f;
    x->x_ctl->c_coef = f * (2 * 3.14159) / x->x_sr;
    if (x->x_ctl->c_coef > 1)
        x->x_ctl->c_coef = 1;
    else if (x->x_ctl->c_coef < 0)
        x->x_ctl->c_coef = 0;
}
static void peak_tilde_clear(t_peak_tilde *x, t_floatarg q)
{
    x->x_cspace.c_x = 0;
}
static t_int *peak_tilde_perform(t_int *w)
{
    t_sample *in = (t_sample *)(w[1]);
    t_sample *out = (t_sample *)(w[2]);
    t_peakctl *c = (t_peakctl *)(w[3]);
    int n = (t_int)(w[4]);
    int i;
    t_sample last = c->c_x;
    t_sample coef = c->c_coef;
    t_sample feedback = 1 - coef;
    for (i = 0; i < n; i++)
        last = *out++ = coef * *in++ + feedback * last;
    if (PD_BIGORSMALL(last))
        last = 0;
    c->c_x = last;
    return (w+5);
}
static void peak_tilde_dsp(t_peak_tilde *x, t_signal **sp)
{
    x->x_sr = sp[0]->s_sr;
    peak_tilde_ft1(x,  x->x_hz);
    dsp_add(peak_tilde_perform, 4,
        sp[0]->s_vec, sp[1]->s_vec,
            x->x_ctl, sp[0]->s_n);
}
void peak_tilde_setup(void)
{
    peak_tilde_class = class_new(gensym("peak~"), (t_newmethod)peak_tilde_new, 0,
        sizeof(t_peak_tilde), 0, A_DEFFLOAT, 0);
    CLASS_MAINSIGNALIN(peak_tilde_class, t_peak_tilde, x_f);
    class_addmethod(peak_tilde_class, (t_method)peak_tilde_dsp,
        gensym("dsp"), A_CANT, 0);
    class_addmethod(peak_tilde_class, (t_method)peak_tilde_ft1,
        gensym("ft1"), A_FLOAT, 0);
    class_addmethod(peak_tilde_class, (t_method)peak_tilde_clear, gensym("clear"), 0);
}

