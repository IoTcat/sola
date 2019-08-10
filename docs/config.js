const langs = [
    { title: '简体中文', path: '/home', matchPath: /^\/(home|ecosystem|support)/ },
];

docute.init({
    landing: 'landing.html',
    title: 'Sola Proj',
    repo: 'iotcat/sola',
    twitter: 'iotcat',
    'edit-link': 'https://github.com/IoTcat/sola/blob/master/docs/',
    nav: {
        default: [
            {
                title: '首页', path: '/en/'
            }
        ],
    },
    plugins: [
        docsearch({
            applicationID: 'AVHNTIOUCA',
            apiKey: 'aec70fc192b330f4e8045e4dcbda9b9f',
            indexName: 'sola',
            tags: ['zh-Hans'],
            url: 'http://wiot.eee.dog'
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
