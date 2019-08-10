
docute.init({
    landing: 'landing.html',
    title: 'Sola Proj',
    repo: 'iotcat/sola',
    twitter: 'iotcat',
    'edit-link': 'https://github.com/IoTcat/sola/blob/master/docs/',
    nav: {
        default: [
            {
                title: '说明文档', path: '/home/'
            },
            {
                title: '开发文档', path: '/home/dev/'
            },
            {
                title: '开发日志', path: '/home/log/'
            },
            {
                title: '捐赠', path: 'https://pay.yimian.xyz'
            }
        ],
    },
    plugins: [
        docsearch({
            applicationID: 'AVHNTIOUCA',
            apiKey: 'aec70fc192b330f4e8045e4dcbda9b9f',
            indexName: 'sola',
            tags: ['zh-Hans'],
            url: 'http://sola.yimian.xyz'
        }),
        evanyou()
    ]
});

function loadScript (src, callback) {
    let s,
        r,
        t;
    r = false;
    s = document.createElement('script');
    s.type = 'text/javascript';
    s.src = src;
    s.onload = s.onreadystatechange = function () {
    // console.log( this.readyState ); //uncomment this line to see which ready states are called.
        if (!r && (!this.readyState || this.readyState == 'complete'))
        {
            r = true;
            callback();
        }
    };
    t = document.getElementsByTagName('script')[0];
    t.parentNode.insertBefore(s, t);
}
